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
	FragAlbedo = vec4(Color.rgb * texture(ColorTexture, TexCoord).rgb, 1);
	FragNormal = ViewNormal.xy;
	FragOthers = vec4(AmbientReflectance, DiffuseReflectance, SpecularReflectance, 1.0f / (SpecularExponent + 1.0f));
}
