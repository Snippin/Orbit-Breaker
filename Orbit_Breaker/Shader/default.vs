#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 Projection;
uniform mat4 View;

out vec4 fColor;

void main()
{
    fColor = aColor;
    gl_Position = Projection * View * vec4(aPos, 1.0);
}
