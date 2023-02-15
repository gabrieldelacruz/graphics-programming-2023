#version 330 core

layout (location = 0) in vec2 ParticlePosition;
layout (location = 1) in float ParticleSize;

// (todo) 02.5: Add Color output variable here


// (todo) 02.X: Add uniforms


void main()
{
	gl_PointSize = ParticleSize;
	gl_Position = vec4(ParticlePosition, 0.0, 1.0);
}
