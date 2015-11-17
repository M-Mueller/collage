#version 330

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 in_position;
in vec4 in_color;

out vec4 fs_color;

void main()
{
	fs_color = in_color;
	gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0);
}
