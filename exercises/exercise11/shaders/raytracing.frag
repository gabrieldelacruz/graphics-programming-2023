//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform mat4 ProjMatrix;
uniform mat4 InvProjMatrix;
uniform uint FrameCount;

void InitRandomSeed();
float Rand01();

void main()
{
	InitRandomSeed();

	// Start from transformed position
	vec4 viewPos = InvProjMatrix * vec4(TexCoord.xy * 2.0f - 1.0f, 0.0f, 1.0f);
	vec3 origin = viewPos.xyz / viewPos.w;

	// Normalize to get view direction
	vec3 dir = normalize(origin);

	// Raytrace the scene
	vec3 color = RayTrace(origin, dir);

	// Compute the alpha to blend between frames of the path tracer
	float alpha = 1.0f / FrameCount;
	FragColor = vec4(color, alpha);
}


// Additional functions

uint RandSeed;

// Initalize random seed
void InitRandomSeed()
{
	uint seedTime = FrameCount;
	uint seedX = uint(gl_FragCoord.x);
	uint seedY = uint(gl_FragCoord.y);
	RandSeed = LCG(seedX) ^ LCG(seedY) ^ LCG(seedTime);
}

// Generates a random float between 0 and 1
float Rand01()
{
	return float(LCG(RandSeed)) / float(0x01000000u);
}

// Returns a random direction on the cosine weighted hemisphere oriented along the normal
vec3 GetDiffuseReflectionDirection(Ray ray, vec3 normal)
{
	float phi = 6.28318530718f * Rand01();
	vec3 direction = GetImplicitNormal(vec2(cos(phi), sin(phi)) * Rand01());
	vec3 bitangent = normalize(cross(normal, normal.z > 0.5f ? vec3(0, 1, 0) : vec3(0, 0, 1)));
	vec3 tangent = cross(normal, bitangent);
	return direction.x * bitangent + direction.y * tangent + direction.z * normal;
}

// Returns the direction of the ray reflected over the normal
vec3 GetSpecularReflectionDirection(Ray ray, vec3 normal)
{
	return reflect(ray.direction, normal);
}

// Returns the direction of the ray refracted 
vec3 GetRefractedDirection(Ray ray, vec3 normal, float f)
{
	return refract(ray.direction, normal, f);
}

vec3 GetAlbedo(Material material)
{
	// Metals have a black albedo
	return mix(material.albedo, vec3(0), material.metalness);
}

vec3 GetReflectance(Material material)
{
	// We use a fixed value for dielectric, with a typical value for these materials (4%)
	return mix(vec3(0.04f), material.albedo, material.metalness);
}

// Schlick simplification of the Fresnel term
vec3 FresnelSchlick(vec3 f0, vec3 viewDir, vec3 halfDir)
{
	return f0 + (vec3(1.0f) - f0) * pow(1.0f - ClampedDot(viewDir, halfDir), 5.0f);
}
