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
    sceneManagement.CreateScene("Scene");
    currentScene = sceneManagement.GetActiveScene();
    auto object1 = CreateGameObject("MarcoPresidente", false);
    auto mesh = Mesh::CreateCube();
    auto meshRenderer = object1->AddComponent<MeshRenderer>();
    meshRenderer->SetMesh(mesh);
    auto image = make_shared<Image>();
    image->LoadTexture("Baker_house.png");
    auto material = make_shared<Material>();
    material->setImage(image);
    meshRenderer->SetMaterial(material);
    currentScene->AddGameObject(object1);

    auto MarcoVicePresidente = CreateGameObject("MarcoVicePresidente", false);
    MarcoVicePresidente->GetTransform()->GetPosition() = vec3(-3, 0, 0);
    auto meshRenderer2 = MarcoVicePresidente->AddComponent<MeshRenderer>();
    auto mesh2 = make_shared<Mesh>();
    auto image2 = make_shared<Image>();
    auto material2 = make_shared<Material>();
    mesh2->LoadMesh("BakerHouse.fbx");
    image2->LoadTexture("Baker_house2.png");
    material2->setImage(image2);
    meshRenderer2->SetMesh(mesh2);
    meshRenderer2->SetMaterial(material2);

    currentScene->AddGameObject(MarcoVicePresidente);

    return true;
}

bool Root::Start() 
{ 
	sceneManagement.Start();

    return true;
}

bool Root::Update(double dt) { 

    /*for (shared_ptr<GameObject> object : children) 
    {
        object->Update(dt);
    }*/

    sceneManagement.Update(dt);

    return true; 
}

shared_ptr<GameObject> Root::CreateMeshObject(string name, shared_ptr<Mesh> mesh)
{
    auto object = CreateGameObject(name, false);

    object->AddComponent<MeshRenderer>();

    auto meshRenderer = object->GetComponent<MeshRenderer>();

    // Load Mesh
    meshRenderer->SetMesh( mesh);

    return nullptr;
}

void Root::RemoveGameObject(std::string name) {
    for (auto it = children.begin(); it != children.end(); ) {
        if ((*it)->GetName() == name) {
            (*it)->Destroy();  // Call Destroy on the object.
            it = children.erase(it); // Erase returns the next iterator.
            return; // Exit after removing the object.
        }
        else {
            ++it; // Move to the next element if not removed.
        }
    }
}

shared_ptr<GameObject> Root::CreateGameObject(string name, bool as_child) {

    string og_name = name;

    int num_repeat = 0;
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]->GetName() == name) {
            num_repeat++;
            name = og_name + std::to_string(num_repeat);

        }
    }

    if (num_repeat != 0) {

        name = og_name + std::to_string(num_repeat);

    }


	shared_ptr<GameObject> object = make_shared<GameObject>(name);

	if (!as_child) {
		children.push_back(object);
	}

	return object;
}

void Root::CreateEmptyObject(std::string name) 
{
	auto go = CreateGameObject(name, false);

}

void Root::CreateCubeObject(std::string name) {

	auto go = CreateGameObject(name, false);
	AddMeshRenderer(*go, Mesh::CreateCube(), "default.png");

}
void Root::CreateSphereObject(std::string name) {
    auto go = CreateGameObject(name, false);
    AddMeshRenderer(*go, Mesh::CreateSphere(), "default.png");

}
void Root::CreatePlaneObject(std::string name) {

    auto go = CreateGameObject(name, false);
    AddMeshRenderer(*go, Mesh::CreatePlane(), "default.png");
}

void Root::AddMeshRenderer(GameObject& go, std::shared_ptr<Mesh> mesh, const std::string& texturePath)
{
    auto meshRenderer = go.AddComponent<MeshRenderer>();
    auto image = std::make_shared<Image>();
    auto material = std::make_shared<Material>();
    image->LoadTexture(texturePath);
    material->setImage(image);
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetMaterial(material);
}
