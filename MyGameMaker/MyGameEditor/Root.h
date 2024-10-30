#pragma once
#include "Module.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/SceneManagement.h"
#include "MyGameEngine/Scene.h"
#include <list>

enum class ObjectType
{
	Empty,
	Cube,
	Sphere,
	Plane,
	Mesh
};

class Root : public Module
{
public:
    Root(App* app);

    virtual ~Root() = default;

    bool Awake();
    bool Start();
    bool PreUpdate() { return true; }
    bool Update(double dt);
    bool PostUpdate() { return true; }

    bool CleanUp() { return true; }

    std::shared_ptr<GameObject> CreateObject(ObjectType type);
    
    std::shared_ptr<GameObject> CreateEmptyObject(std::string name);
    std::shared_ptr<GameObject> CreateCubeObject(std::string name);
    std::shared_ptr<GameObject> CreateSphereObject(std::string name);
    std::shared_ptr<GameObject> CreatePlaneObject(std::string name);

    std::shared_ptr<GameObject> CreateMeshObject(std::string name, std::shared_ptr<Mesh> mesh);

    void RemoveGameObject(std::string name);

public:
    SceneManagement sceneManagement;
    std::shared_ptr<Scene> currentScene = std::make_shared<Scene>("Scene1");
};

