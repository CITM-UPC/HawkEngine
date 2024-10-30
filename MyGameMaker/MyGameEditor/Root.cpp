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

bool  Root::Awake() { 
    
    /*MarcoPresidente = CreateGameObject("MarcoPresidente", false);
    MarcoPresidente->GetTransform()->GetPosition() = vec3(3, 0, 0);
    MarcoPresidente->GetTransform()->Scale(vec3(0.3f, 0.3f, 0.3f));
    auto meshRenderer = MarcoPresidente->AddComponent<MeshRenderer>();
    auto mesh = make_shared<Mesh>();
    auto image = make_shared<Image>();
    auto material = make_shared<Material>();
    mesh->LoadMesh("BakerHouse.fbx");
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);

    MarcoVicePresidente = CreateGameObject("MarcoVicePresidente", false);
    MarcoVicePresidente->GetTransform()->GetPosition() = vec3(-3, 0, 0);
    auto meshRenderer2 = MarcoVicePresidente->AddComponent<MeshRenderer>();
    auto mesh2 = make_shared<Mesh>();
    auto image2 = make_shared<Image>();
    auto material2 = make_shared<Material>();
    mesh2->LoadMesh("BakerHouse.fbx");
    image2->LoadTexture("Baker_house2.png");
    material2->setImage(image2);
    meshRenderer2->SetMesh(mesh2);
    meshRenderer2->SetMaterial(material2);*/

    /*padre = GameObject::CreateEmptyGameObject("Padre");
    padre->GetTransform()->GetPosition() = vec3(0, 0, 0);
    auto meshRendererPadre = padre->AddComponent<MeshRenderer>();
    auto meshPadre = make_shared<Mesh>();
    auto imagePadre = make_shared<Image>();
    auto materialPadre = make_shared<Material>();
    meshPadre->LoadMesh("BakerHouse.fbx");
    imagePadre->LoadTexture("Baker_house.png");
    materialPadre->setImage(imagePadre);
    meshRendererPadre->SetMesh(meshPadre);
    meshRendererPadre->SetMaterial(materialPadre);

    hijo = GameObject::CreateEmptyGameObject("Hijo");
    hijo->GetTransform()->GetPosition() = vec3(10, 0, 0);
    auto meshRendererHijo = hijo->AddComponent<MeshRenderer>();
    auto meshHijo = make_shared<Mesh>();
    auto imageHijo = make_shared<Image>();
    auto materialHijo = make_shared<Material>();
    meshHijo->LoadMesh("BakerHouse.fbx");
    imageHijo->LoadTexture("Baker_house2.png");
    materialHijo->setImage(imageHijo);
    meshRendererHijo->SetMesh(meshHijo);
    meshRendererHijo->SetMaterial(materialHijo);

    padre->emplaceChild(hijo);

    children.push_back(padre);*/
    /*auto object1 = CreateEmptyObject("Cube");
    auto image = make_shared<Image>();
    image->LoadTexture("Baker_house.png");
    auto material = make_shared<Material>();
    material->setImage(image);
    auto meshRenderer = object1->GetComponent<MeshRenderer>();
    meshRenderer->SetMaterial(material);
    currentScene->AddGameObject(object1);*/

   /* auto MarcoVicePresidente = CreateEmptyObject("House");
    MarcoVicePresidente->GetTransform()->GetPosition() = vec3(-3, 0, 0);
    auto meshRenderer2 = MarcoVicePresidente->AddComponent<MeshRenderer>();
    auto mesh2 = make_shared<Mesh>();
    auto image2 = make_shared<Image>();
    auto material2 = make_shared<Material>();
    mesh2->LoadMesh("BakerHouse.fbx");
    image2->LoadTexture("Baker_house2.png");
    material2->setImage(image2);
    meshRenderer2->SetMesh(mesh2);
    meshRenderer2->SetMaterial(material2);*/
    
    sceneManagement.CreateScene("Scene");
    currentScene = sceneManagement.GetActiveScene();

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

    for (auto& go : currentScene->children())
	{
		if (go.HasComponent<MeshRenderer>())
		{
			if (!go.GetComponent<MeshRenderer>()->GetMesh())
			{
				std::cout << "Mesh is null" << std::endl;
                std::cout << "Name: " << go.GetName() << std::endl;
			}
            else
            {
                std::cout << "Mesh is not null" << std::endl;
            }
		}
	}

    return true; 
}

std::shared_ptr<GameObject> Root::CreateObject(ObjectType type)
{
    auto go = make_shared<GameObject>();
    
    switch (type)
	{
	case ObjectType::Empty:
        go = CreateEmptyObject("EmptyGameObject");
		break;
	case ObjectType::Cube:
        go = CreateCubeObject("Cube");
		break;
	case ObjectType::Sphere:
        go = CreateSphereObject("Sphere");
		break;
	case ObjectType::Plane:
        go = CreatePlaneObject("Plane");
		break;
	default:
		break;
	}

    return nullptr;
}

std::shared_ptr<GameObject> Root::CreateEmptyObject(string name)
{
	std::shared_ptr<GameObject> go = make_shared<GameObject>(name);
	
    return go;
}

std::shared_ptr<GameObject> Root::CreateCubeObject(string name)
{
	auto go = make_shared<GameObject>(name);
	go->AddComponent<MeshRenderer>();
	auto meshRenderer = go->GetComponent<MeshRenderer>();
	auto mesh = Mesh::CreateCube();
    auto material = make_shared<Material>();
    auto image = make_shared<Image>();
    image->LoadTexture("Baker_house.png");
    material->setImage(image);
    meshRenderer->SetMaterial(material);
    meshRenderer->SetMesh(mesh);

	return go;
}

std::shared_ptr<GameObject> Root::CreateSphereObject(string name)
{
	auto go = CreateEmptyObject(name);
	go->AddComponent<MeshRenderer>();
	auto meshRenderer = go->GetComponent<MeshRenderer>();
	meshRenderer->SetMesh(Mesh::CreateSphere());

	return go;
}

std::shared_ptr<GameObject> Root::CreatePlaneObject(string name)
{
    auto go = CreateEmptyObject(name);
    go->AddComponent<MeshRenderer>();
    auto meshRenderer = go->GetComponent<MeshRenderer>();
    meshRenderer->SetMesh(Mesh::CreatePlane());

    return go;
}

std::shared_ptr<GameObject> Root::CreateMeshObject(string name, shared_ptr<Mesh> mesh)
{
    auto go = CreateEmptyObject(name);
    go->AddComponent<MeshRenderer>();
    auto meshRenderer = go->GetComponent<MeshRenderer>();
    meshRenderer->SetMesh(mesh);

    return nullptr;
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