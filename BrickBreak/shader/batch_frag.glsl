#version 450 core

layout (location = 0) out vec4 color;

uniform sampler2D uTextures[10];

in DATA {
	vec4 color;
	vec2 uv;
	float slot;
} fsData;

void main()
{
    int slot = int(fsData.slot + 0.5f);
    /* if (slot >= 0) */
	color = fsData.color * texture(uTextures[slot], fsData.uv);
    /* else */
        /* color = fsData.color; */
    /* if (slot == 0) color = fsData.color * texture(brick, fsData.uv); */
    /* if (slot == 1) color = fsData.color * texture(solid, fsData.uv); */
    /* if (slot == 2) color = fsData.color * texture(paddle, fsData.uv); */
    /* if (slot == 3) color = fsData.color * texture(ball, fsData.uv); */
}
