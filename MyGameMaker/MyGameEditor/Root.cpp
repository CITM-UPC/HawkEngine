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
    auto mesh = std::make_shared<Mesh>();
    mesh->LoadMesh("BakerHouse.fbx");
    AddMeshRenderer(go, mesh, "Baker_house.png");

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

void Root::CreateEmptyObject(std::string name)
{
	auto& go = currentScene->emplaceChild();
	go.SetName(name);
}

void Root::CreateCubeObject(std::string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    AddMeshRenderer(go, Mesh::CreateCube());
}

void Root::CreateSphereObject(std::string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    AddMeshRenderer(go, Mesh::CreateSphere());
}

void Root::CreatePlaneObject(std::string name)
{
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    AddMeshRenderer(go, Mesh::CreatePlane());
}

void Root::CreateMeshObject(std::string name, std::shared_ptr<Mesh> mesh)
{
    if (!mesh) {
        std::cerr << "Error: Invalid mesh provided to CreateMeshObject" << std::endl;
        return;
    }
    
    auto& go = currentScene->emplaceChild();
    go.SetName(name);
    AddMeshRenderer(go, mesh);
}

void Root::AddMeshRenderer(GameObject& go, std::shared_ptr<Mesh> mesh, const std::string& texturePath)
{
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto image = std::make_shared<Image>();
    auto material = std::make_shared<Material>();
    image->LoadTexture(texturePath);
    material->setImage(image);
    meshRenderer->SetImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
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