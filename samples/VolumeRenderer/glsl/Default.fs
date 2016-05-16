#version 330

uniform bool perVertexColor = false;
uniform vec4 color = vec4(1.0);

in vec4 fs_color;
out vec4 out_color;

void main()
{
    if(perVertexColor)
	   out_color = fs_color;
    else
        out_color = color;
}
