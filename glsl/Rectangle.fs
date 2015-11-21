#version 330

uniform sampler2D tex;

in vec2 fs_texCoord;
out vec4 out_color;

void main()
{
	out_color = texture(tex, fs_texCoord);
}
