#version 150 core

in vec3 v_Color;
in vec2 v_Texcoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
	//outColor = vec4(1-Color.r, 1 - Color.g, 1 - Color.b, 1.0);
	//outColor = vec4(Color, 1.0);
	outColor = texture(tex, v_Texcoord) * vec4(v_Color, 1.0);
}