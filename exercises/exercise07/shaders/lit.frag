//Inputs
in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform vec3 Color;
uniform sampler2D ColorTexture;

uniform float AmbientReflectance;
uniform float DiffuseReflectance;
uniform float SpecularReflectance;
uniform float SpecularExponent;

uniform vec3 CameraPosition;

void main()
{
	SurfaceData data;
	data.normal = normalize(WorldNormal);
	data.reflectionColor = Color * texture(ColorTexture, TexCoord).rgb;
	data.ambientReflectance = AmbientReflectance;
	data.diffuseReflectance = DiffuseReflectance;
	data.specularReflectance = SpecularReflectance;
	data.specularExponent = SpecularExponent;

	vec3 position = WorldPosition;
	vec3 viewDir = GetDirection(position, CameraPosition);
	vec3 color = ComputeLighting(position, data, viewDir, true);
	FragColor = vec4(color.rgb, 1);
}
