#version 330

uniform sampler2D rayEntryTex;
uniform sampler3D volume;
uniform float step;

uniform int mode; // 0: iso, 1: maximum
uniform float iso;

in vec3 fs_rayExit;
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

struct Ray
{
	vec3 position;
	vec3 direction;
	vec4 color;

	// set to true if the ray can be terminated early
	bool terminate;

	// variables of the maximum mode
	float maximum;
};

Ray isoSurfaceStep(float value, Ray ray)
{
	if(value >= iso)
	{
		int refinementDir = -1;
		for(int i=1; i<6; ++i)
		{
			ray.position += refinementDir*step/(pow(2, i)) * ray.direction;
			float refinedValue = texture(volume, ray.position.rgb).r;
			if(value > iso)
				refinementDir = -1;
			else if(value < iso)
				refinementDir = 1;
			else
				break;
		}

		vec3 normal = computeNormal(ray.position);
		ray.color = vec4(computePhongShading(ray.position, normal), 1.0);
		ray.terminate = true;
	}
	else
	{
		ray.color = ray.color;
		ray.terminate = false;
	}
	return ray;
}

Ray maximumIntensityStep(float value, Ray ray)
{
	if(value > ray.maximum)
	{
		ray.maximum = value;
		ray.color = vec4(ray.maximum);
	}
	return ray;
}

void main()
{
	vec3 rayEnter = texture(rayEntryTex, gl_FragCoord.xy/textureSize(rayEntryTex, 0)).rgb;
	vec3 rayExit = fs_rayExit;

	Ray ray;
	ray.position = rayEnter;
	ray.direction = normalize(rayExit - rayEnter);
	ray.color = vec4(0.0);
	ray.terminate = false;
	ray.maximum = 0.0;

	for(int i=0; i<1000; ++i)
	{
		// FIXME: because of floating point errors, the first step is sometimes skipped
		if(i > 0 && (any(lessThan(ray.position, vec3(0.0))) || any(greaterThan(ray.position, vec3(1.0)))))
		{
			break;
		}

		float value = texture(volume, ray.position).r;
		if(mode == 0)
			ray = isoSurfaceStep(value, ray);
		else if(mode == 1)
			ray = maximumIntensityStep(value, ray);

		if(ray.terminate)
			break;

		ray.position += step*ray.direction;
	}
	out_color = ray.color;
}
