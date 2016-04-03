#version 330

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

out vec3 fs_rayExit;

void main()
{
	fs_rayExit = in_color.rgb;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
