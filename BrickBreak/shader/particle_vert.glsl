#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out DATA {
    vec2 uv;
} vsData;

uniform mat4 uProjection;
uniform vec2 uOffset;

void main()
{
    float scale = 10.0f;
    vsData.uv = uv;
    gl_Position = uProjection * vec4((pos * scale) + uOffset, -1.0, 1.0);
}
