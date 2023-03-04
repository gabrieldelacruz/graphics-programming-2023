#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 WorldPosition;
out vec3 WorldNormal;
out vec2 TexCoord;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;
uniform sampler2D Heightmap;

void main()
{
	float height = texture(Heightmap, (VertexPosition.xz * 127 + 0.5f) / 128).r;
	WorldPosition = (WorldMatrix * vec4(VertexPosition.x, height, VertexPosition.z, 1.0)).xyz;
	WorldNormal = (WorldMatrix * vec4(VertexNormal, 0.0)).xyz;
	TexCoord = VertexTexCoord;
	gl_Position = ViewProjMatrix * vec4(WorldPosition, 1.0);
}
