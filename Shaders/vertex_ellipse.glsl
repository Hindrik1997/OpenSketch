#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texturecoord;

uniform mat4 transformMatrix;

out vec2 texcoord;

void main()
{
    gl_Position = transformMatrix * vec4(position.x, position.y, position.z, 1.0);
    texcoord = texturecoord;
}