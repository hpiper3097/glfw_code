#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

uniform float aRed;
uniform float aGreen;
uniform float aBlue;

void main()
{
	FragColor = vec4(aRed, aGreen, aBlue, 1.0f);
}