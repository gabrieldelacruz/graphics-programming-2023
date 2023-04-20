
// Uniforms
// TODO 10.1 : Replace constants with uniforms with the same name
uniform vec3 SphereColor = vec3(0, 0, 1);
uniform vec3 SphereCenter = vec3(-2, 0, -10);
uniform float SphereRadius = 1.25f;

uniform vec3 BoxColor = vec3(1, 0, 0);
uniform mat4 BoxMatrix = mat4(1,0,0,0,   0,1,0,0,   0,0,1,0,   2,0,-10,1);
uniform vec3 BoxSize = vec3(1, 1, 1);

// Output structure
struct Output
{
	// color of the closest figure
	vec3 color;
};


// Signed distance function
float GetDistance(vec3 p, inout Output o)
{
	// Sphere in position "SphereCenter" and size "SphereRadius"
	float dSphere = SphereSDF(TransformToLocalPoint(p, SphereCenter), SphereRadius);

	// Box with worldView transform "BoxMatrix" and dimensions "BoxSize"
	float dBox = BoxSDF(TransformToLocalPoint(p, BoxMatrix), BoxSize);

	// TODO 10.2 : Replace Union with SmoothUnion and try different small values of smoothness
	float d = Union(dSphere, dBox);

	// TODO 10.2 : Replace this with a mix, using the blend factor from SmoothUnion
	o.color = d == dSphere ? SphereColor : BoxColor;

	return d;
}

// Default value for o
void InitOutput(out Output o)
{
	o.color = vec3(0.0f);
}

// Output function: Just a dot with the normal and view vectors
vec4 GetOutputColor(vec3 p, float distance, Output o)
{
	vec3 normal = CalculateNormal(p);
	vec3 viewDir = normalize(-p);
	float dotNV = dot(normalize(-p), normal);
	return vec4(dotNV * o.color, 1.0f);
}
