#include "Root.h"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/TransformComponent.h"
#include "MyGameEngine/MeshRendererComponent.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/Image.h"
#include "MyGameEngine/Material.h"

#include <iostream>

using namespace std;

Root::Root(App* app) : Module(app) { ; }

bool  Root::Awake() 
{  
    sceneManagement.CreateScene("Scene");
    currentScene = sceneManagement.GetActiveScene();

    CreateObject(ObjectType::Cube);


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

void Root::CreateEmptyObject(string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
}

void Root::CreateCubeObject(string name)
{
	auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = make_shared<Mesh>();
    auto image = make_shared<Image>();
    auto material = make_shared<Material>();
    mesh = Mesh::CreateCube();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}

void Root::CreateSphereObject(string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = make_shared<Mesh>();
    auto image = make_shared<Image>();
    auto material = make_shared<Material>();
    mesh = Mesh::CreateSphere();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}

void Root::CreatePlaneObject(string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto mesh = make_shared<Mesh>();
    auto image = make_shared<Image>();
    auto material = make_shared<Material>();
    mesh = Mesh::CreatePlane();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}

void Root::CreateMeshObject(string name, shared_ptr<Mesh> mesh)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto meshComponent = mesh;
    auto material = make_shared<Material>();
    meshRenderer->SetMesh(meshComponent);
    meshRenderer->SetMaterial(material);
}

void Root::RemoveGameObject(std::string name)
{
	for (auto& go : currentScene->children())
	{
		if (go.GetName() == name)
		{
			//currentScene->removeChild(go);
			break;
		}
	}
}