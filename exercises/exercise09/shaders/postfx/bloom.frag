//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;
uniform vec2 Range;
uniform float Intensity;

void main()
{
	vec3 color = texture(SourceTexture, TexCoord).rgb;

	// Compute the luminance and divide the color by the value
	float luminance = GetLuminance(color);

	// Remap the luminance to the valid range, and clamp it between 0 and 1
	float result = (luminance - Range.x) / max(Range.y - Range.x, 0.0001f);
	result = clamp(result, 0.0f, 1.0f);

	// Return the color multiplied by the result, scaled by the intensity
	FragColor = vec4(color * result * Intensity, 1.0f);
}
