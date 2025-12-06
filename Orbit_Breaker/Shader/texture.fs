#version 460 core

uniform sampler2D Textures[8];

in vec4 fColor;
in vec2 fTexCoords;
in float fTexId;

out vec4 color;

void main()
{
    if (fTexId > 0)
    {
        int id = int(fTexId);
        color = fColor * texture(Textures[id], fTexCoords);
    }
    else
    {
        color = fColor;
    }
}
