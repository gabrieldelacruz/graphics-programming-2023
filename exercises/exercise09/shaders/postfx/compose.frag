//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;
uniform float Exposure;

void main()
{
	// Read from the HDR framebuffer
	vec3 hdrColor = texture(SourceTexture, TexCoord).rgb;

	// Apply exposure
	vec3 color = vec3(1.0f) - exp(-hdrColor * Exposure);

	// Assign the fragment color
	FragColor = vec4(color, 1.0f);
}
