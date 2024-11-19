#include "GameObject.h"
#include "MeshRendererComponent.h"
#include "../MyGameEditor/App.h"
//#include "../MyGameEditor/Root.h"
#include <iostream>

GameObject::GameObject(const std::string& name) : name(name), cachedComponentType(typeid(Component)) 
{
    components = std::map<std::type_index, ComponentVariant>();

    transform = &AddComponent<Transform_Component>();

    int i = 9;

}

GameObject::~GameObject()
{
    //for (auto& component : components) {
    //    component.second->Destroy();
    //}
    //components.clear();

    //for (auto& child : children()) {
    //    child.Destroy();
    //}
}

// comment all the use of components to check if the error persists

void GameObject::Start()
{

    transform = &AddComponent<Transform_Component>();

    //for (auto& component : components)
    //{
    //    component.second->Start();
    //}

    //for (auto& child : children())
    //{
    //    child.Start();
    //}
}

void GameObject::Update(float deltaTime)
{
   if (!active)
   {
       return;
   }

   for (auto& component : components)
   {
       //return std::get<component.first>(components.at(typeid(component.first)));

       std::visit([&](auto&& comp) {
           comp.Update(deltaTime);  // Revise this later
           }, component.second);
   }
   
   for (auto& child : children())
   {
	    child.Update(deltaTime);
   }

   Draw();
}

void GameObject::Destroy()
{

    Application->root->RemoveGameObject(name);
    
    //if (this) {

    //    destroyed = true;

    //    for (auto& component : components)
    //    {
    //        component.second->Destroy();
    //    }

    //    for (auto& child : children())
    //    {
    //        child.Destroy();
    //    }
    //}
}

void GameObject::Draw() 
{
    //std::cout << "Draw GameObject: " << name << std::endl;
    
    switch (drawMode)
    {
    case DrawMode::AccumultedMatrix:
        DrawAccumultedMatrix();
        break;
    case DrawMode::InstancedMatrix:
        DrawInstancedMatrix();
        break;
    case DrawMode::PushPopMatrix:
        DrawPushPopMatrix();
        break;
    }
}

void GameObject::DrawAccumultedMatrix() const
{
    //De momento nada ya lo hare en un futuro :)
}

void GameObject::DrawInstancedMatrix() const
{
	//De momento nada ya lo hare en un futuro :)
}

void GameObject::DrawPushPopMatrix() 
{
    glPushMatrix();
    glMultMatrixd(transform->GetData());

    if (HasComponent<MeshRenderer>()) {
        GetComponent<MeshRenderer>();
        MeshRenderer meshRenderer = GetComponent<MeshRenderer>();
        
        meshRenderer.Render();
    }

    glPopMatrix();
}

void GameObject::OnEnable() {}

void GameObject::OnDisable() {}

void GameObject::SetActive(bool active)
{
    this->active = active;

    if (active)
    {
        OnEnable();
    }
    else
    {
        OnDisable();
    }
}

bool GameObject::IsActive() const
{
    return active;
}

std::string GameObject::GetName() const
{
    return name;
}

void GameObject::SetName(const std::string& name)
{
    this->name = name;
}

bool GameObject::CompareTag(const std::string& tag) const
{
	return this->tag == tag;
}

BoundingBox GameObject::boundingBox() const 
{
    BoundingBox bbox = localBoundingBox();
    if (!mesh && children().size()) bbox = children().front().boundingBox();
    for (const auto& child : children()) bbox = bbox + child.boundingBox();
    return transform->GetMatrix() * bbox;
}