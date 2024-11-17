#include "Scene.h"

#include "MeshRendererComponent.h"
#include "Mesh.h"

void Scene::Start()
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		(*it)->Start();
	}
}

void Scene::Update(float deltaTime)
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

void Scene::Destroy()
{

	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		(*it)->Destroy();
	}
}

void Scene::OnEnable()
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		(*it)->OnEnable();
	}
}

void Scene::OnDisable()
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		(*it)->OnDisable();
	}
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	// implement like in Root legacy implementation
	//removeChild(*gameObject);
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	//_children.push_back(gameObject);
}

std::shared_ptr<GameObject> Scene::FindGameObjectbyName(const std::string& name) 
{
	for (auto& child : _children)
	{
		if (child->GetName() == name)
		{
			return std::make_shared<GameObject>(child);
		}
	}
	return nullptr;
}

std::shared_ptr<GameObject> Scene::FindGameObjectbyTag(const std::string& tag)
{
	for (auto& child : _children)
	{
		if (child->CompareTag(tag))
		{
			return std::make_shared<GameObject>(child);
		}


	}

	return nullptr;
}

std::string Scene::GetName() const
{
	return name;
}

void Scene::SetName(const std::string& name)
{
	this->name = name;
}

//std::shared_ptr<GameObject> Scene::CreateEmptyGameObject(const std::string& name)
//{
//	auto object = std::make_shared<GameObject>(name);
//	return object;
//}
//
//std::shared_ptr<GameObject> Scene::CreateCube(const std::string& name)
//{
//	auto object = std::make_shared<GameObject>(name);
//	object->AddComponent<MeshRenderer>();
//	auto meshRenderer = object->GetComponent<MeshRenderer>();
//	meshRenderer->SetMesh(Mesh::CreateCube());
//	return object;
//}
//
//std::shared_ptr<GameObject> Scene::CreateSphere(const std::string& name)
//{
//	auto object = std::make_shared<GameObject>(name);
//	object->AddComponent<MeshRenderer>();
//	auto meshRenderer = object->GetComponent<MeshRenderer>();
//	meshRenderer->SetMesh(Mesh::CreateSphere());
//	return object;
//}
//
//std::shared_ptr<GameObject> Scene::CreatePlane(const std::string& name)
//{
//	auto object = std::make_shared<GameObject>(name);
//	object->AddComponent<MeshRenderer>();
//	auto meshRenderer = object->GetComponent<MeshRenderer>();
//	meshRenderer->SetMesh(Mesh::CreatePlane());
//	return object;
//}