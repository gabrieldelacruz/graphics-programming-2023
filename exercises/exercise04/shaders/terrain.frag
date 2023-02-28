#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in float Height;

out vec4 FragColor;

uniform vec4 Color;

uniform sampler2D ColorTexture0;
uniform vec2 ColorTexture01Range;
uniform sampler2D ColorTexture1;
uniform vec2 ColorTexture12Range;
uniform sampler2D ColorTexture2;
uniform vec2 ColorTexture23Range;
uniform sampler2D ColorTexture3;

uniform vec2 ColorTextureScale;

float inverseMix(vec2 range, float value)
{
	return clamp((value - range.x) / (range.y - range.x), 0, 1);
}

void main()
{
	vec4 color0 = texture(ColorTexture0, TexCoord * ColorTextureScale);
	vec4 color1 = texture(ColorTexture1, TexCoord * ColorTextureScale);
	vec4 color2 = texture(ColorTexture2, TexCoord * ColorTextureScale);
	vec4 color3 = texture(ColorTexture3, TexCoord * ColorTextureScale);

	vec4 color = color0;
	color = mix(color, color1, inverseMix(ColorTexture01Range, Height));
	color = mix(color, color2, inverseMix(ColorTexture12Range, Height));
	color = mix(color, color3, inverseMix(ColorTexture23Range, Height));

	FragColor = Color * color;
}
