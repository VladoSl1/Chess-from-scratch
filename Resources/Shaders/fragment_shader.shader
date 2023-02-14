#version 330 core

out vec4 o_Color;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec3 modelColor;

void main()
{
	vec4 texColor = vec4(modelColor, 1.0) * texture(texture1, TexCoords);
	if (texColor.a < 0.1)
		discard;
	o_Color = texColor;
}