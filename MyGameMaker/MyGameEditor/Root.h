#pragma once
#include "Module.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/SceneManagement.h"
#include "MyGameEngine/Scene.h"
#include <list>



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

    std::shared_ptr<GameObject> CreateMeshObject(std::string name, std::shared_ptr<Mesh> mesh);

    std::shared_ptr<GameObject> CreateGameObject(std::string name, bool as_child = false);

    void RemoveGameObject(std::string name);

public:

	std::vector< std::shared_ptr<GameObject> > children;
    
    SceneManagement sceneManagement;
    std::shared_ptr<Scene> currentScene = std::make_shared<Scene>("Scene1");
};

