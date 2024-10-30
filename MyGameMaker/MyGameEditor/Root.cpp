#include "Root.h"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/TransformComponent.h"
#include "MyGameEngine/MeshRendererComponent.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/Image.h"
#include "MyGameEngine/Material.h"

#include <iostream>

Root::Root(App* app) : Module(app) { ; }

bool  Root::Awake() 
{  
    sceneManagement.CreateScene("Scene");
    currentScene = sceneManagement.GetActiveScene();

    auto& go = currentScene->emplaceChild();
    go.SetName("House");
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = std::make_shared<Mesh>();
    auto image = std::make_shared<Image>();
    auto material = std::make_shared<Material>();
    mesh->LoadMesh("BakerHouse.fbx");
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);

    return true;
}

bool Root::Start() 
{ 
    sceneManagement.Start();


    return true;
}

bool Root::Update(double dt) 
{   
    sceneManagement.Update(dt);

    return true; 
}

void Root::CreateObject(ObjectType type)
{    
    switch (type)
	{
	case ObjectType::Empty:
        CreateEmptyObject("EmptyGameObject");
		break;
	case ObjectType::Cube:
        CreateCubeObject("Cube");
		break;
	case ObjectType::Sphere:
        CreateSphereObject("Sphere");
		break;
	case ObjectType::Plane:
        CreatePlaneObject("Plane");
		break;
	default:
		break;
	}
}

void Root::CreateEmptyObject(std::string name)
{
	auto& go = currentScene->emplaceChild();
	go.SetName(name);
}

void Root::CreateCubeObject(std::string name)
{
	auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = std::make_shared<Mesh>();
    auto image = std::make_shared<Image>();
    auto material = std::make_shared<Material>();
    mesh = Mesh::CreateCube();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}

void Root::CreateSphereObject(std::string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = std::make_shared<Mesh>();
    auto image = std::make_shared<Image>();
    auto material = std::make_shared<Material>();
    mesh = Mesh::CreateSphere();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}

void Root::CreatePlaneObject(std::string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = std::make_shared<Mesh>();
    auto image = std::make_shared<Image>();
    auto material = std::make_shared<Material>();
    mesh = Mesh::CreatePlane();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}

void Root::CreateMeshObject(std::string name, std::shared_ptr<Mesh> mesh)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    meshRenderer->SetMesh(mesh);
}

void Root::RemoveGameObject(std::string gameObject)
{
	for (auto& go : currentScene->children())
	{
		if (go.GetName() == gameObject)
		{
			//currentScene->removeChild(go);
			break;
		}
	}
}