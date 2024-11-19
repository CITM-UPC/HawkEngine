#pragma once
#include <type_traits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <concepts>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

class GameObject;
class SceneSerializer;

class Component
{
public:
	explicit Component(GameObject* owner) : owner(owner) {}

	//Component() { ; }

	virtual ~Component() {

		int debug = 0;
	};

	virtual void Start() ;
	virtual void Update(float deltaTime) ;
	virtual void Destroy() ;

	GameObject& GetOwner() const { return *owner; }
	std::string GetName() const { return name; }

	bool IsEnabled() const { return enabled; }
	void SetEnabled(bool enabled) { this->enabled = enabled; }

	void SetName(const std::string& name) { this->name = name; }

protected:

	friend class SceneSerializer;

	GameObject* owner;
	std::string name;
	bool enabled = true;

	bool IsOwnerValid() const { return owner; }


	virtual YAML::Node encode() {
		YAML::Node node;

		node["name"] = name;
		node["enabled"] = enabled;


		return node;
	}

};

template <typename T>
concept IsComponent = std::is_base_of<Component, T>::value;