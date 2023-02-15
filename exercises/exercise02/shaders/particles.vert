#version 330 core

layout (location = 0) in vec2 ParticlePosition;
layout (location = 1) in float ParticleSize;
layout (location = 2) in float ParticleBirth;
layout (location = 3) in float ParticleDuration;
layout (location = 4) in vec4 ParticleColor;

out vec4 Color;

uniform float CurrentTime;

void main()
{
	Color = ParticleColor;
	float age = CurrentTime - ParticleBirth;
	gl_PointSize = age < ParticleDuration ? ParticleSize : 0;
	gl_Position = vec4(ParticlePosition, 0.0, 1.0);
}
