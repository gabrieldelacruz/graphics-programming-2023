//Inputs
layout (location = 0) in vec3 VertexPosition;

//Outputs
out vec2 TexCoord;

void main()
{
	// texture coordinates
	TexCoord = VertexPosition.xy * 0.5f + 0.5f;

	// final vertex position (for rendering, not for lighting)
	gl_Position = vec4(VertexPosition.xy, -1.0, 1.0);
}
