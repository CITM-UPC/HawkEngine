#pragma once


#include "Component.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "TreeExt.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include <memory>
#include <variant>

class SceneSerializer;

using ComponentVariant =  std::variant<Transform_Component, MeshRenderer>;

enum class DrawMode
{
    AccumultedMatrix,
    InstancedMatrix,
    PushPopMatrix
};

class GameObject : public std::enable_shared_from_this<GameObject>, public TreeExt<GameObject>
{
public:
    GameObject(const std::string& name = "GameObject");
    ~GameObject();

    template <IsComponent T, typename... Args>
    T& AddComponent(Args&&... args);
    // add a variant instance of Component ,  and then "cast it  to the type" Component<Mesh> mesh;
    // normally it would be deleted outside it's scope but it won't if you store it in a vector
    // then to delet it just pop it out of the vector

    template <IsComponent T>
    T& GetComponent() ;

    template <IsComponent T>
    void RemoveComponent();

    template <IsComponent T>
    bool HasComponent() const;

    //void Awake();
    void Start();
    //void FixedUpdate(float fixedDeltaTime);
    void Update(float deltaTime);
    //void LateUpdate(float deltaTime);
    void Draw() ;
    void Destroy();

    void OnEnable();
    void OnDisable();

    void SetActive(bool active);
    bool IsActive() const;

    std::string GetName() const;
    void SetName(const std::string& name);

    bool CompareTag(const std::string& tag) const;

    BoundingBox boundingBox() const;
    BoundingBox localBoundingBox() const { 
        return mesh ? mesh->boundingBox() : BoundingBox(); 
    
    
    }

    Transform_Component& GetTransform() const { return *transform; }

    //Primitive Creation
    static std::shared_ptr<GameObject> CreateEmptyGameObject(const std::string& name = "EmptyGameObject");
    static std::shared_ptr<GameObject> CreateCube(const std::string& name = "Cube");
    static std::shared_ptr<GameObject> CreateSphere(const std::string& name = "Sphere");
    static std::shared_ptr<GameObject> CreatePlane(const std::string& name = "Plane");
    static std::shared_ptr<GameObject> CreateQuad(const std::string& name = "Quad");
    static std::shared_ptr<GameObject> CreateCylinder(const std::string& name = "Cylinder");
    static std::shared_ptr<GameObject> CreateCapsule(const std::string& name = "Capsule");

    DrawMode drawMode = DrawMode::PushPopMatrix;

    bool operator==(const GameObject& other) const {
        return name == other.name;
    }



private:

    friend class SceneSerializer;

    void DrawAccumultedMatrix() const;
    void DrawInstancedMatrix() const;
    void DrawPushPopMatrix() ;

    std::string name;
    std::string tag = "Untagged";
    bool active = true;
    bool destroyed = false;




    //std::vector< std::unique_ptr<Component>> components;

    // get component use dynamic_cast 
    // std:.variant <some types> , a variant can be any of the specified types

   // std::unordered_map< int, std::unique_ptr<Component>> components;
   
    std::map <std::type_index, ComponentVariant> components;

    Transform_Component* transform;
    Mesh* mesh;

    mutable std::type_index cachedComponentType;
    Component* cachedComponent;



public:

    bool isSelected = false;

};


template <IsComponent T, typename... Args>
T& GameObject::AddComponent(Args&&... args) {
   

    //throw std::runtime_error("Component not found on GameObject: " + this->GetName()); 

    
    /*std::unique_ptr newComponent = std::make_unique<T>(this);
    components[typeid(T)] = std::move(newComponent);*/

    //ComponentVariant newComponent = T(this);
    ComponentVariant newComponent(std::in_place_type<T>, this);
     
    components[typeid(T)] = newComponent;

    return std::get<T>(components.at(typeid(T)));

    /*T newComponent = T(this);

    components[typeid(T)] = dynamic_cast<ComponentVariant>(T);

    return newComponent;*/
}

template <IsComponent T>
T& GameObject::GetComponent()  {

    

    if (cachedComponentType == typeid(T)) {

        if (HasComponent<T>()){

            return dynamic_cast<T&>(cachedComponent);
        }
    }
    // remember nothing can have a ptr to a unique_ptr

    auto it = components.find(typeid(T));
    if (it != components.end()) {

        return std::get<T>(components.at(typeid(T)));

        /*cachedComponentType = typeid(T);

        cachedComponent = it->second;

        return dynamic_cast<T&>(cachedComponent);*/
    }
    else {
        throw std::runtime_error("Component not found on GameObject: " + this->GetName());
    }
}

//en un futuro si el componente esta siendo usado no dejar que se elimine
template <IsComponent T>
void GameObject::RemoveComponent() {
    //auto it = components.find(typeid(T));
    //if (it != components.end()) {
    //    components.erase(it);
    //}
    //else {
    //    //Log a warning or something idk asdsa
    //}
}

template <IsComponent T>
bool GameObject::HasComponent() const {

    return components.find(typeid(T)) != components.end();
}

