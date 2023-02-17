#version 330 core

layout (location = 0) in vec2 ParticlePosition;
layout (location = 1) in float ParticleSize;
layout (location = 2) in float ParticleBirth;
layout (location = 3) in float ParticleDuration;
layout (location = 4) in vec4 ParticleColor;
layout (location = 5) in vec2 ParticleVelocity;

out vec4 Color;

uniform float CurrentTime;
uniform float Gravity;

void main()
{
	Color = ParticleColor;
	float age = CurrentTime - ParticleBirth;
	gl_PointSize = age < ParticleDuration ? ParticleSize : 0;

	// initial position (x0)
	vec2 position = ParticlePosition;
	// add velocity: initial velocity (v0) * age
	position += ParticleVelocity * age;
	// add gravity: 1/2  gravity (a) * age^2
	position += 0.5f * vec2(0, Gravity) * age * age;
	gl_Position = vec4(position, 0.0, 1.0);
}
