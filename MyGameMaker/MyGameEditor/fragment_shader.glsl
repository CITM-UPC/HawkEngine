#version 450 core

in vec2 TexCoord; // Texture coordinate of the fragment
in vec3 FragPos; // Position of the fragment
in vec3 Normal; // Normal of the fragment
in vec4 ExtraColor;

out vec4 FragColor;

uniform sampler2D texture1; // Texture sampler

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

#define MAX_POINT_LIGHTS 4

uniform PointLight pointLights;
uniform DirLight dirLight;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // Point lights
   
    result += CalcPointLight(pointLights, norm, FragPos, viewDir);
    

    // Sample the texture using the texture coordinates
    vec4 texColor = texture(texture1, TexCoord) + ExtraColor;
    FragColor = vec4(result, 1.0) * texColor;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // Combine results
    vec3 ambient = light.ambient * light.intensity;
    vec3 diffuse = light.diffuse * diff * light.intensity;
    vec3 specular = light.specular * spec * light.intensity;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation *= smoothstep(light.radius, 0.0, distance); // Apply radius attenuation
    // Combine results
    vec3 ambient = light.ambient * light.intensity * attenuation;
    vec3 diffuse = light.diffuse * diff * light.intensity * attenuation;
    vec3 specular = light.specular * spec * light.intensity * attenuation;
    return (ambient + diffuse + specular);
}