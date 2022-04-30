#version 460 core

in DATA {
    vec2 uv;
} fsData;

uniform vec4 uColor;
uniform sampler2D uSprite;

layout (location = 0) out vec4 color;

void main()
{
    color = texture(uSprite, fsData.uv) * uColor;
}
