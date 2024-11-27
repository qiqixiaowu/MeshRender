#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;  

uniform mat4 modelMat;
layout (std140) uniform Matrices
{
	mat4 viewMat;
	mat4 projMat;
 };

out vec3 Normal;
out vec3 Position;
out vec3 vertexColor; 

void main()
{
    Normal = mat3(transpose(inverse(modelMat))) * aNormal;
    Position = vec3(modelMat * vec4(aPos, 1.0));
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
    vertexColor = aColor;
}