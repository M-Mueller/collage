#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

out vec3 fs_rayExit;

void main()
{
	fs_rayExit = in_color.rgb;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
