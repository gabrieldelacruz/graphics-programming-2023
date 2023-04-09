
uniform samplerCube EnvironmentTexture;
uniform float EnvironmentMaxLod;

struct SurfaceData
{
	vec3 normal;
	vec3 albedo;
	float ambientOcclusion;
	float roughness;
	float metalness;
};

// Constant value for PI
const float Pi = 3.1416f;

// Constant value for 1 / PI
const float invPi = 0.31831f;

// Get the surface albedo
vec3 GetAlbedo(SurfaceData data)
{
	// (todo) 08.7: Adjust albedo with metalness


	return data.albedo;
}

// Get the surface reflectance
vec3 GetReflectance(SurfaceData data)
{
	// (todo) 08.7: Get reflectance value for metals


	// We use a fixed value for dielectric, with a typical value for these materials (4%)
	return vec3(0.04f);
}

// Schlick simplification of the Fresnel term
vec3 FresnelSchlick(vec3 f0, vec3 viewDir, vec3 halfDir)
{
	return f0 + (vec3(1.0f) - f0) * pow(1.0f - ClampedDot(viewDir, halfDir), 5.0f);
}

// GGX equation for distribution function
float DistributionGGX(vec3 normal, vec3 halfDir, float roughness)
{
	// (todo) 08.5: Implement the equation
	return vec3(0.0f);
}

// Geometry term in one direction, for GGX equation
float GeometrySchlickGGX(float cosAngle, float roughness)
{
	// (todo) 08.6: Implement the equation
	return vec3(1.0f);
}

// Geometry term in both directions, following Smith simplification, that divides it in the product of both directions
float GeometrySmith(vec3 normal, vec3 inDir, vec3 outDir, float roughness)
{
	// Occlusion in input direction (shadowing)
	float ggxIn = GeometrySchlickGGX(ClampedDot(normal, inDir), roughness);

	// Occlusion in output direction (masking)
	float ggxOut = GeometrySchlickGGX(ClampedDot(normal, outDir), roughness);

	// Total occlusion is a product of both
	return ggxIn * ggxOut;
}

// Sample the EnvironmentTexture cubemap
// lodLevel is a value between 0 and 1 to select from the highest to the lowest mipmap
vec3 SampleEnvironment(vec3 direction, float lodLevel)
{
	// Flip the Z direction, because the cubemap is left-handed
	direction.z *= -1;

	// Sample the specified mip-level
	return textureLod(EnvironmentTexture, direction, lodLevel * EnvironmentMaxLod).rgb;
}

vec3 ComputeDiffuseIndirectLighting(SurfaceData data)
{
	// Sample the environment map at its max LOD level and multiply with the albedo
	return SampleEnvironment(data.normal, 1.0f) * GetAlbedo(data);
}

vec3 ComputeSpecularIndirectLighting(SurfaceData data, vec3 viewDir)
{
	// Compute the reflection vector with the viewDir and the normal
	vec3 reflectionDir = reflect(-viewDir, data.normal);

	// Sample the environment map using the reflection vector, at a specific LOD level
	float lodLevel = pow(data.roughness, 0.25f);
	vec3 specularLighting = SampleEnvironment(reflectionDir, lodLevel);

	// (todo) 08.6: Add a geometry term to the indirect specular


	return specularLighting;
}

vec3 CombineIndirectLighting(vec3 diffuse, vec3 specular, SurfaceData data, vec3 viewDir)
{
	// Compute the Fresnel term between the normal and the view direction
	vec3 fresnel = FresnelSchlick(GetReflectance(data), viewDir, data.normal);

	// Linearly interpolate between the diffuse and specular term, using the fresnel value
	return mix(diffuse, specular, fresnel) * data.ambientOcclusion;
}

vec3 ComputeDiffuseLighting(SurfaceData data, vec3 lightDir)
{
	// Implement the lambertian equation for diffuse
	return GetAlbedo(data) * invPi;
}

vec3 ComputeSpecularLighting(SurfaceData data, vec3 lightDir, vec3 viewDir)
{
	// (todo) 08.5: Implement the Cook-Torrance equation using the D (distribution) and G (geometry) terms
	return vec3(0.0f);
}

vec3 CombineLighting(vec3 diffuse, vec3 specular, SurfaceData data, vec3 lightDir, vec3 viewDir)
{
	// Compute the Fresnel term between the half direction and the view direction
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 fresnel = FresnelSchlick(GetReflectance(data), viewDir, halfDir);

	// Linearly interpolate between the diffuse and specular term, using the fresnel value
	vec3 lighting = mix(diffuse, specular, fresnel);

	// Move the incidence factor to affect the combined light value
	float incidence = ClampedDot(data.normal, lightDir);
	lighting *= incidence;

	return lighting;
}
