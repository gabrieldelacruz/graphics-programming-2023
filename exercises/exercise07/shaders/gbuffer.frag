//Inputs
in vec3 ViewNormal;
in vec2 TexCoord;

//Outputs
out vec4 FragAlbedo;
out vec2 FragNormal;
out vec4 FragOthers;

//Uniforms
uniform vec3 Color;
uniform sampler2D ColorTexture;

uniform float AmbientReflectance;
uniform float DiffuseReflectance;
uniform float SpecularReflectance;
uniform float SpecularExponent;

void main()
{
	FragAlbedo = vec4(0.0f);
	FragNormal = vec2(0.0f);
	FragOthers = vec4(0.0f);
}
