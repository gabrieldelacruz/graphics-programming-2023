//Inputs
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTangent;
layout (location = 3) in vec3 VertexBitangent;
layout (location = 4) in vec2 VertexTexCoord;

//Outputs
out vec3 ViewNormal;
out vec3 ViewTangent;
out vec3 ViewBitangent;
out vec2 TexCoord;

//Uniforms
uniform mat4 WorldViewMatrix;
uniform mat4 WorldViewProjMatrix;

void main()
{
	// normal in view space (for lighting computation)
	ViewNormal = (WorldViewMatrix * vec4(VertexNormal, 0.0)).xyz;

	// tangent in view space (for lighting computation)
	ViewTangent = (WorldViewMatrix * vec4(VertexTangent, 0.0)).xyz;

	// bitangent in view space (for lighting computation)
	ViewBitangent = (WorldViewMatrix * vec4(VertexBitangent, 0.0)).xyz;

	// texture coordinates
	TexCoord = VertexTexCoord;

	// final vertex position (for opengl rendering, not for lighting)
	gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);
}
