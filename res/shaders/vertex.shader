#version 150 core

in vec2 position;
in vec3 color;
in vec2 texcoord;

out vec3 v_Color;
out vec2 v_Texcoord;

void main()
{
	v_Color = color;
	v_Texcoord = texcoord;
	gl_Position = vec4(position, 0.0, 1.0);
}