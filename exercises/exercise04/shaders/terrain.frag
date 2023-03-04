#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in float Height;

out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D ColorTexture0;
uniform sampler2D ColorTexture1;
uniform sampler2D ColorTexture2;
uniform sampler2D ColorTexture3;
uniform vec2 ColorTextureRange01;
uniform vec2 ColorTextureRange12;
uniform vec2 ColorTextureRange23;
uniform vec2 ColorTextureScale;

// Return a value between 0 and 1 depending of the position of value in range
float inverseMix(vec2 range, float value)
{
	return clamp((value - range.x) / (range.y - range.x), 0, 1);
}

void main()
{
	// Sample all the color textures
	vec4 color0 = texture(ColorTexture0, TexCoord * ColorTextureScale);
	vec4 color1 = texture(ColorTexture1, TexCoord * ColorTextureScale);
	vec4 color2 = texture(ColorTexture2, TexCoord * ColorTextureScale);
	vec4 color3 = texture(ColorTexture3, TexCoord * ColorTextureScale);

	// Mix between them according to height ranges
	vec4 color = color0;
	color = mix(color, color1, inverseMix(ColorTextureRange01, Height));
	color = mix(color, color2, inverseMix(ColorTextureRange12, Height));
	color = mix(color, color3, inverseMix(ColorTextureRange23, Height));

	// Multiply texture color by vertex Color
	FragColor = Color * color;
}
