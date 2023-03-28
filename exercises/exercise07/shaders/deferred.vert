//Inputs
layout (location = 0) in vec3 VertexPosition;

//Outputs
out vec2 TexCoord;

//Uniforms
uniform mat4 WorldViewProjMatrix;

void main()
{
	// final vertex position (for opengl rendering, not for lighting)
	gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);

	// texture coordinates
	TexCoord = (gl_Position.xy / gl_Position.w) * 0.5f + 0.5f;
}
