#version 450 core

layout(location = 0) in vec3 aPos; // Position attribute
layout(location = 1) in vec2 aTexCoord; // Texture coordinate attribute
layout(location = 2) in vec3 aNormal; // Normal attribute
layout(location = 3) in vec4 aColor; // Color attribute

out vec2 TexCoord; // Texture coordinate of the fragment
out vec3 FragPos; // Position of the fragment
out vec3 Normal; // Normal of the fragment
out vec4 VertexColor; // Color of the vertex

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Pass the texture coordinates to the fragment shader
    TexCoord = aTexCoord;

    // Pass the fragment position to the fragment shader
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Pass the normal to the fragment shader
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass the color to the fragment shader
    VertexColor = aColor;

    // Transform the vertex position to clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}