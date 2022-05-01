#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec2 offset;
layout (location = 3) in vec4 color;

out DATA {
    vec2 uv;
    vec4 color;
} vsData;

uniform mat4 uProjection;

void main()
{
    float scale = 10.0f;
    vsData.uv = uv;
    vsData.color = color;
    gl_Position = uProjection * vec4((pos * scale) + offset, -1.0, 1.0);
}
