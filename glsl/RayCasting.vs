#version 330

uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

layout(location = 0) in vec3 in_position;

out vec3 fs_rayExit;

void main()
{
	fs_rayExit = (vec4(in_position, 1.0)).xyz;
	gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0);
}
