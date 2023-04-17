
// Transformations ---

// Transform point relative to a specific position
vec3 TransformToLocalPoint(vec3 p, vec3 position)
{
	return p - position;
}

// Transform point relative to a transform matrix
vec3 TransformToLocalPoint(vec3 p, mat4 m)
{
	return (inverse(m) * vec4(p, 1)).xyz;
}

// Transform vector relative to a transform matrix
vec3 TransformToLocalVector(vec3 v, mat4 m)
{
	return (inverse(m) * vec4(v, 0)).xyz;
}

// SDFs ---

// Signed distance field of a plane
float PlaneSDF(vec3 p, vec3 normal, float offset)
{
	return dot(p, normal) - offset;
}

// Signed distance field of a sphere
float SphereSDF(vec3 p, float radius)
{
	return length(p) - radius;
}

// Signed distance field of a sphere with analytic normal
float SphereSDF(vec3 p, out vec3 normal, float radius)
{
	float distance = length(p);
	normal = p / distance;
	return distance - radius;
}

// Signed distance field of a box
float BoxSDF(vec3 p, vec3 halfsize)
{
	vec3 q = abs(p) - halfsize;
	float outerDist = length(max(q, 0.0f));
	float innerDist = min(max(q.x, max(q.y, q.z)), 0.0f);
	return outerDist + innerDist;
}

// Signed distance field of a cylinder
float CylinderSDF(vec3 p, float height, float radius)
{
	vec2 d = abs(vec2(length(p.xz), p.y)) - vec2(radius, height);
	float outerDist = length(max(d, 0.0f));
	float innerDist = min(max(d.x, d.y), 0.0f);
	return outerDist + innerDist;
}

// Signed distance field of a torus
float TorusSDF(vec3 p, float mainRadius, float sideRadius)
{
	vec2 q = vec2(length(p.xz) - mainRadius, p.y);
	return length(q) - sideRadius;
}

// Operations ---

float Invert(float d)
{
	return -d;
}

// Union of two SDFs
float Union(float a, float b)
{
	return min(a, b);
}

// Intersection of two SDFs
float Intersection(float a, float b)
{
	return max(a, b);
}

// Substract SDF b from SDF a
float Substraction(float a, float b)
{
	return max(a, -b);
}

// Smooth union with smoothness k
float SmoothUnion(float a, float b, float k)
{
	float h = max(k - abs(a - b), 0.0) / k;
	return min(a, b) - h * h * k * (1.0 / 4.0);
}

// Smooth union with smoothness k and returning blend value in range (0-1)
float SmoothUnion(float a, float b, float k, out float blend)
{
	vec3 v = max(vec3(k) - vec3(b, a, abs(a - b)), 0.0f) / k;
	blend = (v.x*v.x - v.y*v.y);

	blend = blend * 0.5f + 0.5f;
	float h = v.z;
	return min(a, b) - h * h * k * (1.0 / 4.0);
}

