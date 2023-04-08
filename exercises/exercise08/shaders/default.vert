//Inputs
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTangent;
layout (location = 3) in vec3 VertexBitangent;
layout (location = 4) in vec2 VertexTexCoord;

//Outputs
out vec3 WorldPosition;
out vec3 WorldNormal;
out vec3 WorldTangent;
out vec3 WorldBitangent;
out vec2 TexCoord;

//Uniforms
uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;

void main()
{
	// vertex position in world space (for lighting computation)
	WorldPosition = (WorldMatrix * vec4(VertexPosition, 1.0)).xyz;

	// normal in world space (for lighting computation)
	WorldNormal = (WorldMatrix * vec4(VertexNormal, 0.0)).xyz;

	// tangent in world space (for lighting computation)
	WorldTangent = (WorldMatrix * vec4(VertexTangent, 0.0)).xyz;

	// bitangent in world space (for lighting computation)
	WorldBitangent = (WorldMatrix * vec4(VertexBitangent, 0.0)).xyz;

	// texture coordinates
	TexCoord = VertexTexCoord;

	// final vertex position (for opengl rendering, not for lighting)
	gl_Position = ViewProjMatrix * vec4(WorldPosition, 1.0);
}
