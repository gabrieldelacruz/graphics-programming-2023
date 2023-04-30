//Inputs
in vec3 ViewNormal;
in vec3 ViewTangent;
in vec3 ViewBitangent;
in vec2 TexCoord;
in vec3 ViewPosition;

//Outputs
out vec4 FragAlbedo;
out vec2 FragNormal;
out vec4 FragOthers;

//Uniforms
uniform vec3 Color;
uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;

void main()
{
	FragAlbedo = vec4(Color.rgb * texture(ColorTexture, TexCoord).rgb, 1);

	vec3 viewNormal = SampleNormalMap(NormalTexture, TexCoord, normalize(ViewNormal), normalize(ViewTangent), normalize(ViewBitangent));

	vec3 viewDir = GetDirection(ViewPosition, vec3(0));
	vec3 tangent = cross(viewDir, vec3(0,1,0));
	vec3 bitangent = cross(viewDir, tangent);
	FragNormal = vec2(dot(viewNormal, tangent), dot(viewNormal, bitangent));

	FragOthers = texture(SpecularTexture, TexCoord);
}
