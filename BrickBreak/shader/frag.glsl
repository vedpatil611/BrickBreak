#version 460 core

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 color;

uniform sampler2D image;
uniform vec4 uColor;

void main()
{
    color = uColor * texture(image, uv);
}
