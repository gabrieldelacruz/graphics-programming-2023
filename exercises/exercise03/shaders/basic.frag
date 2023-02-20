#version 330 core

// (todo) 03.X: Add the in variables here


out vec4 FragColor;

uniform vec3 Color = vec3(1);

void main()
{
	FragColor = vec4(Color, 1);
}
