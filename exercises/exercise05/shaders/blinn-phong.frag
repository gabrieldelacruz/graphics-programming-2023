#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D ColorTexture;

uniform float AmbientReflection;
uniform float DiffuseReflection;

uniform vec3 AmbientColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;

vec3 GetAmbientReflection(vec3 objectColor)
{
	return AmbientColor * AmbientReflection * objectColor;
}

vec3 GetDiffuseReflection(vec3 objectColor, vec3 lightVector, vec3 normalVector)
{
	return LightColor * DiffuseReflection * objectColor * max(dot(lightVector, normalVector), 0.0f);
}

vec3 GetBlinnPhongReflection(vec3 objectColor, vec3 lightVector, vec3 normalVector)
{
	return GetAmbientReflection(objectColor) + GetDiffuseReflection(objectColor, lightVector, normalVector);
}

void main()
{
	vec4 objectColor = Color * texture(ColorTexture, TexCoord);
	vec3 lightVector = normalize(LightPosition - WorldPosition);
	vec3 normalVector = normalize(WorldNormal);
	FragColor = vec4(GetBlinnPhongReflection(objectColor.rgb, lightVector, normalVector), 1.0f);
}
