

struct Ray
{
	vec3 point;
	vec3 direction;
	vec3 colorFilter;
	float ior;
};

// Forward declare distance function
vec3 CastRay(Ray ray, inout float distance);
vec3 CastRay(Ray ray)
{
	const float infinity = 1.0f/0.0f;
	float distance = infinity;
	return CastRay(ray, distance);
}

// Forward declare config function
void GetRayTracerConfig(out uint maxRays);

// Hard limit for the number of rays. Affects performance
const uint RayCapacity = 16u;
Ray _PendingRays[RayCapacity];

uint _RayCount = 0u;
uint _RayIndex = 0u;
uint _RayMaxCount = RayCapacity;

bool PushRay(in Ray ray)
{
	bool pushed = false;
	if (_RayCount < _RayMaxCount) // && dot(ray.colorFilter, ray.colorFilter) > 0.0f)
	{
		// Offset in the ray direction
		ray.point += 0.0001f * ray.direction;
		_PendingRays[_RayCount++] = ray;
		pushed = true;
	}
	return pushed;
}


bool GetPendingRay(out Ray ray)
{
	bool found = false;
	if (_RayIndex < _RayCount && _RayIndex < _RayMaxCount)
	{
		ray = _PendingRays[_RayIndex++];
		found = true;
	}
	return found;
}

vec3 RayTrace(vec3 point, vec3 direction)
{
	vec3 color = vec3(0);

	uint maxRays;
	GetRayTracerConfig(maxRays);
	_RayMaxCount = min(_RayMaxCount, maxRays);

	Ray ray = Ray(point, direction, vec3(1.0f), 1.0f);

	do
	{
		color += CastRay(ray);
	} while(GetPendingRay(ray));

	return color;
}
