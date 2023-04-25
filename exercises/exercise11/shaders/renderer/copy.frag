//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;

void main()
{
	FragColor = texture(SourceTexture, TexCoord);
}
