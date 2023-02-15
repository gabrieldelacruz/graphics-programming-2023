#version 330 core

out vec4 FragColor;

in vec4 Color;

void main()
{
	float alpha = 1 - length(gl_PointCoord * 2 - 1);

	FragColor = vec4(Color.rgb, Color.a * alpha);
}
