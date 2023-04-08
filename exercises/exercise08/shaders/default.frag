//Inputs
in vec3 WorldPosition;
in vec3 WorldNormal;
in vec3 WorldTangent;
in vec3 WorldBitangent;
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform vec3 Color;
uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;

uniform vec3 CameraPosition;

void main()
{
	SurfaceData data;
	data.normal = SampleNormalMap(NormalTexture, TexCoord, normalize(WorldNormal), normalize(WorldTangent), normalize(WorldBitangent));
	data.reflectionColor = Color * texture(ColorTexture, TexCoord).rgb;
	vec3 arm = texture(SpecularTexture, TexCoord).rgb;
	data.ambientReflectance = arm.x;
	data.diffuseReflectance = 1.0f;
	data.specularReflectance = pow(1.0f - arm.y, 4);
	data.specularExponent = 2.0f / pow(arm.y, 2) - 2.0f;

	vec3 position = WorldPosition;
	vec3 viewDir = GetDirection(position, CameraPosition);
	vec3 color = ComputeLighting(position, data, viewDir, true);
	FragColor = vec4(color.rgb, 1);
}
