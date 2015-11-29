#version 330

uniform sampler2D backface;
uniform sampler2D frontface;
uniform sampler3D volume;
uniform float iso;
uniform float step;

in vec2 fs_texCoord;
out vec4 out_color;

vec3 computePhongShading(vec3 pos, vec3 normal)
{
	vec3 viewer = vec3(0.0);
	vec3 light = vec3(10.0, 10.0, 10.0);
	vec3 toViewer = normalize(pos - viewer);
	vec3 toLight = normalize(pos - light);

	vec3 ambient = vec3(0.05, 0.05, 0.05);
	vec3 diffuse = vec3(1.0, 1.0, 1.0) * dot(toLight, normal);

	return ambient + diffuse;
}

vec3 computeNormal(vec3 pos)
{
	vec4 offset = vec4(1.0/32.0, 1.0/32.0, 1.0/32.0, 0);
	float dx = texture(volume, pos.rgb - offset.xww).r - texture(volume, pos.rgb + offset.xww).r;
	float dy = texture(volume, pos.rgb - offset.wyw).r - texture(volume, pos.rgb + offset.wyw).r;
	float dz = texture(volume, pos.rgb - offset.wwz).r - texture(volume, pos.rgb + offset.wwz).r;
	return vec3(dx/2, dy/2, dz/2);
}

void main()
{
	vec3 rayEnter = texture(frontface, fs_texCoord).rgb;
	vec3 rayExit = texture(backface, fs_texCoord).rgb;

	vec3 rayOrigin = rayEnter;
	vec3 rayDirection = normalize(rayExit - rayEnter);

	for(int i=0; i<1000; ++i)
	{
		if(any(lessThan(rayOrigin, vec3(0.0))) || any(greaterThan(rayOrigin, vec3(1.0))))
		{
			out_color = vec4(0.0);
			break;
		}

		float value = texture(volume, rayOrigin.rgb).r;
		if(value >= iso)
		{
			int refinementDir = -1;
			for(int i=1; i<6; ++i)
			{
				rayOrigin += refinementDir*step/(pow(2, i)) * rayDirection;
				float refinedValue = texture(volume, rayOrigin.rgb).r;
				if(value > iso)
					refinementDir = -1;
				else if(value < iso)
					refinementDir = 1;
				else
					break;
			}

			vec3 normal = computeNormal(rayOrigin);
			out_color = vec4(computePhongShading(rayOrigin, normal), 1.0);
			break;
		}

		rayOrigin += step*rayDirection;
	}
}
