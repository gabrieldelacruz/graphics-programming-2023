//Inputs
layout (location = 0) in vec3 VertexPosition;

//Uniforms
uniform mat4 WorldViewProjMatrix;

void main()
{
	gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);
}
