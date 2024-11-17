#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "TreeExt.h"
#include "Mesh.h"
#include "BoundingBox.h"

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

    template <IsComponent T>
    T& GetComponent() const ;

    template <IsComponent T>
    void RemoveComponent();

    template <IsComponent T>
    bool HasComponent() const;

    //void Awake();
    void Start();
    //void FixedUpdate(float fixedDeltaTime);
    void Update(float deltaTime);
    //void LateUpdate(float deltaTime);
    void Draw() const;
    void Destroy();

    void OnEnable();
    void OnDisable();

    void SetActive(bool active);
    bool IsActive() const;

    std::string GetName() const;
    void SetName(const std::string& name);

    bool CompareTag(const std::string& tag) const;

    BoundingBox boundingBox() const;
    BoundingBox localBoundingBox() const { return mesh ? mesh->boundingBox() : BoundingBox(); }

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

    void DrawAccumultedMatrix() const;
    void DrawInstancedMatrix() const;
    void DrawPushPopMatrix() const;

    std::string name;
    std::string tag = "Untagged";
    bool active = true;
    bool destroyed = false;

    std::unordered_map<std::type_index, Component* > components;

    //std::list<std::shared_ptr<Component>> components;

    mutable std::type_index cachedComponentType;
    mutable std::shared_ptr<Component> cachedComponent;

    std::shared_ptr<Transform_Component> transform;
    std::shared_ptr<Mesh> mesh;

public:

    bool isSelected = false;

};


template <IsComponent T, typename... Args>
T& GameObject::AddComponent(Args&&... args) {
    //std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>(args)...);

    //// This stores the shared pointer in the components map
    //components[typeid(T)] = newComponent;
    //return *newComponent;

    T* newComponent = new T(args...);
    components[typeid(T)] = newComponent;
    return *newComponent;

}

template <IsComponent T>
T& GameObject::GetComponent() const {
    // Find the component in the map
    auto it = components.find(typeid(T));

    if (it != components.end()) {
        // Retrieve the raw pointer from the map
        T* componentPtr = static_cast<T*>(it->second);

        if (componentPtr) {
            return *componentPtr;  // Return the referenced component
        }
        else {
            throw std::runtime_error("Failed to cast component to the desired type.");
        }
    }

    throw std::runtime_error("Component not found on GameObject: " + this->GetName());
}
//T& GameObject::GetComponent() const{
//    /* Find the component in the map */
//    auto it = components.find(typeid(T));
//
//    if (it) {
//        return (T) *it;
//    }
//
//    //if (it != components.end()) {
//    //    auto derived_ptr = std::dynamic_pointer_cast<T>(it->second);
//
//    //    if (!derived_ptr) {
//    //        throw std::runtime_error("Failed to cast component to the desired type.");
//    //    }
//
//    //    return *derived_ptr;  /* Return reference to the component */
//    //}
//    throw std::runtime_error("Component not found on GameObject: " + this->GetName());
//}


//template <IsComponent T>
//T* GameObject::GetComponent() const {
//    if (cachedComponentType == typeid(T) && cachedComponent) {
//        return std::static_pointer_cast<T>(cachedComponent);
//    }
//
//    auto it = components.find(typeid(T));
//    if (it != components.end()) {
//        cachedComponentType = typeid(T);
//        *cachedComponent = it->second;
//        return std::static_pointer_cast<T>(cachedComponent);
//    }
//    else {
//
//
//        throw std::runtime_error("Component not found on GameObject: " + this->GetName());
//    }
//}



//en un futuro si el componente esta siendo usado no dejar que se elimine
template <IsComponent T>
void GameObject::RemoveComponent() {
    auto it = components.find(typeid(T));
    if (it != components.end()) {
        components.erase(it);
    }
    else {
        //Log a warning or something idk asdsa
    }
}

template <IsComponent T>
bool GameObject::HasComponent() const {
    return components.find(typeid(T)) != components.end();
}