#include "GameObject.h"
#include "MeshRendererComponent.h"
#include <string>
#include <iostream>

//GameObject::GameObject(const char* Aname) noexcept : name(std::string(Aname)), cachedComponentType(typeid(Component))
//{
//    
//}

GameObject::~GameObject()
{
    for (auto& component : components) {
        component.second->Destroy();
    }
    components.clear();

    for (auto& child : _children) {
        child->Destroy();
    }
}

void GameObject::Start()
{
    // no make_unique in the constructor bc it can be marked as noexcept and then the vector class cries
    transform = std::make_unique<Transform_Component>(AddComponent<Transform_Component>());

    for (auto& component : components)
    {
        component.second->Start();
    }

    for (auto& child : _children)
    {
        child->Start();
    }
}

void GameObject::Update(float deltaTime)
{
    if (!active)
    {
        return;
    }

    for (auto& component : components)
    {
        component.second->Update(deltaTime);
    }
    
    for (auto& child : _children)
	{
		child->Update(deltaTime);
	}

    Draw();
}

void GameObject::Destroy()
{
    destroyed = true;
    
    for (auto& component : components)
	{
		component.second->Destroy();
	}

    for (auto& child : _children)
    {
        child->Destroy();
    }
}

void GameObject::Draw() const
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

void GameObject::DrawPushPopMatrix() const
{
    glPushMatrix();
    glMultMatrixd(transform->GetData());

    if (HasComponent<MeshRenderer>())
    {
        MeshRenderer meshRenderer = GetComponent<MeshRenderer>();
        //          second = dictionary value
       /* meshRenderer->second->Render();*/


        /**meshRenderer->Render();*/
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
    if (!mesh && _children.size()) bbox = _children.front()->boundingBox();
    for (const auto& child : _children) {

        bbox = bbox + child->boundingBox();
    }
    return transform->GetMatrix() * bbox;
}