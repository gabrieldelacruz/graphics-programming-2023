//Inputs
in vec2 TexCoord;

//Outputs
out vec4 FragColor;

//Uniforms
uniform sampler2D SourceTexture;

uniform float Exposure;

uniform float Contrast;
uniform float HueShift;
uniform float Saturation;
uniform vec3 ColorFilter;

uniform sampler2D BloomTexture;

vec3 AdjustContrast(vec3 color)
{
	color = (color - vec3(0.5f)) * Contrast + vec3(0.5f);
	return clamp(color, 0, 1);
}

vec3 AdjustHue(vec3 color)
{
	vec3 hsvColor = RGBToHSV(color);
	hsvColor.x = fract(hsvColor.x + HueShift + 1.0f);
	return HSVToRGB(hsvColor);
}

vec3 AdjustSaturation(vec3 color)
{
	vec3 luminance = vec3(GetLuminance(color));
	color = (color - luminance) * Saturation + luminance;
	return clamp(color, 0, 1);
}

vec3 ApplyColorFilter(vec3 color)
{
	return color * ColorFilter;
}


void main()
{
	// Read from the HDR framebuffer
	vec3 hdrColor = texture(SourceTexture, TexCoord).rgb;

	// Add bloom
	hdrColor += texture(BloomTexture, TexCoord).rgb;

	// Apply exposure
	vec3 color = vec3(1.0f) - exp(-hdrColor * Exposure);

	// Color grading
	color = AdjustContrast(color);
	color = AdjustHue(color);
	color = AdjustSaturation(color);
	color = ApplyColorFilter(color);

	// Assign the fragment color
	FragColor = vec4(color, 1.0f);
}
