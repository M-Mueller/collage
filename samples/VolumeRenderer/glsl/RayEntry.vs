#version 330

uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform bool usePositionAsColor;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec4 fs_color;

void main()
{
    if(usePositionAsColor)
        fs_color = vec4(in_position, 1.0);
    else
        fs_color = vec4(in_color, 1.0);
    gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0);
}
