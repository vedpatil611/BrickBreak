#version 460 core

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    color = spriteColor * texture(image, uv);
}
