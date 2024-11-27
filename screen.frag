#version 430 core
									
in vec4 vertexColor;
in vec2 TexCoords;

uniform sampler2D renderedTexture;
out vec4 FragColor;								   

void main()
{
	FragColor= texture(renderedTexture,TexCoords);
}