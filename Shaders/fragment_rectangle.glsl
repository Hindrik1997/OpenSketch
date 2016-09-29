#version 330 core

out vec4 color;
in vec2 texcoord;

uniform int width;
uniform int height;
uniform bool isSelected;

void main()
{
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    //10 pixel border
    float bordery = 1.0f / width * 10;
    float borderx = 1.0f / height * 10;

    if(isSelected && (texcoord.y <= bordery || texcoord.y >= 1 - bordery || texcoord.x <= borderx || texcoord.x >= 1 - borderx ))
    {
        color = vec4(0.48f, 0.66f, 0.93f, 1.0f);
    }
}