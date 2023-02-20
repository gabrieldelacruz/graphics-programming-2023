#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

// (todo) 03.X: Add the out variables here


// (todo) 03.X: Add the uniforms here


void main()
{
	gl_Position = vec4(VertexPosition, 1.0);
}
