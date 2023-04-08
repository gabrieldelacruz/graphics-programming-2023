#version 330 core

//Inputs
in vec3 ViewDir;

//Outputs
out vec4 FragColor;

//Uniforms
uniform samplerCube SkyboxTexture;

void main()
{
	// Sample always the highest LOD
	FragColor = textureLod(SkyboxTexture, ViewDir, 0);
}
