#version 450 core

const int MAX_SPRITES = 1000;

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 color;
layout (location = 3) in float slot;
layout (location = 4) in float index;

out DATA {
    vec4 color;
    vec2 uv;
    float slot;
} vsData;

uniform mat4 uModel[MAX_SPRITES];
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uModel[int(index)] * vec4(pos, 0.0, 1.0);
    vsData.color = color;
    vsData.uv = uv;
    vsData.slot = slot;
}
