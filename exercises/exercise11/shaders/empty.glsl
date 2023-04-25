
// Output structure
struct Output
{
	// color of the closest figure
	vec3 color;
};

// Signed distance function
float GetDistance(vec3 p, inout Output o)
{
	return 1000000.0f;
}

// Default value for o
void InitOutput(out Output o)
{
	o.color = vec3(1.0f);
}

// Output function
vec4 GetOutputColor(vec3 p, float distance, Output o)
{
	return vec4(o.color, 1.0f);
}
