#version 330

uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 texCoord;

out vec3 fs_color;
out vec3 fs_texCoord;

void main()
{
	fs_color = color;
	fs_texCoord = texCoord;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0, 1.0);
}
