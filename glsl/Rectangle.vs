#version 330

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 fs_texCoord;

void main()
{
	fs_texCoord = texCoord;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0, 1.0);
}
