#pragma once
#include "Component.h"
#include "TransformComponent.h"

class LightComponent : public Component {
public:

    enum LightType
    {
        DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
    };

    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
        float radius;
        float intensity;
    };

    struct DirLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float intensity;
    };

    explicit LightComponent();
    ~LightComponent() override = default;

    void Start() override;
    void Update(float deltaTime) override;
    void Destroy() override;
  
private:
    

};