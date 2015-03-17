#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV; 
layout(location = 2) in vec3 vertexNormal;
uniform vec3 selectionColor;
uniform mat4 MVP;

out vec2 UV;
out vec3 normal;
out vec3 fragmentColor;

void main()
{
	vec4 v = vec4(vertexPosition_modelspace, 1);
	gl_Position = MVP * v;
	UV = vertexUV;
	normal = vertexNormal;
	fragmentColor = selectionColor;
}