#version 330 core

out vec4 color;
in vec2 texcoord;

uniform int width;
uniform int height;
uniform bool isSelected;

void main()
{
    color = vec4(0.0f, 1.0f, 0.05f, 1.0f);

    //10 pixel border
    float bordery = 1.0f / width * 10;
    float borderx = 1.0f / height * 10;

    //simpele functie om ellipse te make
    float length = length(texcoord - vec2(0.5, 0.5));
    //discarden van de pixel als hij niet getekent hoeft te worden. (buiten radius valt, wat 0.5 is, aangezien de ellipse vlakvullend is)
    if (length > 0.5)
    {
        discard;
    }

    if(isSelected &&  length > 0.45)
    {
        color = vec4(0.48f, 0.66f, 0.93f, 1.0f);
        return;
    }
}