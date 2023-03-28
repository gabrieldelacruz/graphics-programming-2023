//Inputs
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

//Outputs
out vec3 WorldPosition;
out vec3 WorldNormal;
out vec2 TexCoord;

//Uniforms
uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;

void main()
{
	// vertex position in world space (for lighting computation)
	WorldPosition = (WorldMatrix * vec4(VertexPosition, 1.0)).xyz;

	// normal in world space (for lighting computation)
	WorldNormal = normalize((WorldMatrix * vec4(VertexNormal, 0.0)).xyz);

	// texture coordinates
	TexCoord = VertexTexCoord;

	// final vertex position (for opengl rendering, not for lighting)
	gl_Position = ViewProjMatrix * vec4(WorldPosition, 1.0);
}
