#version 330 core

//Inputs
layout (location = 0) in vec3 VertexPosition;

//Outputs
out vec3 ViewDir;

//Uniforms
uniform vec3 CameraPosition;
uniform mat4 InvViewProjMatrix;

void main()
{
	// Use always max depth
	gl_Position = vec4(VertexPosition.xy, 1.0f, 1.0f);

	// Get the world position using the inverse view-projection matrix
	vec4 worldPosition = InvViewProjMatrix * gl_Position;

	// Projection is linear in homogeneous coordinates, need to divide by W
	worldPosition /= worldPosition.w;

	// Get the view direction
	ViewDir = worldPosition.xyz - CameraPosition;

	// Cubemap coordinates are left-handed, and world is right-handed, requires flip Z
	ViewDir.z = -ViewDir.z;
}
