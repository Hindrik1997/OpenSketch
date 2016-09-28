#version 330 core

out vec4 color;
in vec2 texcoord;

uniform int width;
uniform int height;
uniform bool isSelected;

const int pixels = 10;

void main()
{
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    float xsegment = 1.0f / width;
    float ysegment = 1.0f / height;

    if(isSelected && (texcoord.x <= (xsegment * pixels) || texcoord.x >= 1.0f - (xsegment * pixels) ||
                      texcoord.y <= (ysegment * pixels) || texcoord.y >= 1.0f - (ysegment * pixels)))
    {
        color = vec4(0.48f, 0.66f, 0.93f, 1.0f);
    }
}