#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform vec4 Offset;

void main()
{
    gl_Position = vec4(aPos.x + Offset.x, aPos.y + Offset.y, aPos.z + Offset.z, 1.0 + Offset.w);
	vertexColor = vec4(aColor, 1.0);
	TexCoord = aTexCoord;
}