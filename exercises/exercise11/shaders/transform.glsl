
// Transform point relative to a specific position
vec3 TransformToLocalPoint(vec3 p, vec3 position)
{
	return p - position;
}

// Transform point relative to a specific position back to view space
vec3 TransformFromLocalPoint(vec3 p, vec3 position)
{
	return p + position;
}

// Transform point relative to a transform matrix
vec3 TransformToLocalPoint(vec3 p, mat4 m)
{
	return (inverse(m) * vec4(p, 1)).xyz;
}

// Transform point relative to a transform matrix back to view space
vec3 TransformFromLocalPoint(vec3 p, mat4 m)
{
	return (m * vec4(p, 1)).xyz;
}

// Transform vector relative to a transform matrix
vec3 TransformToLocalVector(vec3 v, mat4 m)
{
	return (inverse(m) * vec4(v, 0)).xyz;
}

// Transform vector relative to a transform matrix back to view space
vec3 TransformFromLocalVector(vec3 v, mat4 m)
{
	return (m * vec4(v, 0)).xyz;
}
