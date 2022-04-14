#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec2 texCoords;

uniform mat4 uModel = mat4(1.0f);
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uModel * vec4(pos, 0.0, 1.0);
    texCoords = uv;
}
