
//
vec3 GetCameraPosition(mat4 viewMatrix)
{
	vec3 position = viewMatrix[3].xyz;
	position = -(transpose(viewMatrix) * vec4(position, 0)).xyz;
	return position;
}

//
vec3 GetHalfVector(vec3 v1, vec3 v2)
{
   return normalize(v1 + v2);
}

//
vec3 GetDirection(vec3 fromPosition, vec3 toPosition)
{
	return normalize(toPosition - fromPosition);
}

//
float ClampedDot(vec3 v1, vec3 v2)
{
	return max(dot(v1, v2), 0);
}

vec3 GetImplicitNormal(vec2 normal)
{
	// (todo) 07.3: Obtain the implicit Z component of the normal
	return vec3(0);
}

//
vec3 ReconstructViewPosition(sampler2D depthTexture, vec2 texCoord, mat4 invProjMatrix)
{
	// (todo) 07.4: Reconstruct the position, using the screen texture coordinates and the depth
	return vec3(0);
}

