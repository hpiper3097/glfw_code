#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;

uniform vec4 Offset;

void main()
{
    gl_Position = vec4(aPos.x + Offset.x, aPos.y + Offset.y, aPos.z + Offset.z, 1.0 + Offset.w);
	vertexColor = gl_Position;
}