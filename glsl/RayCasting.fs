#version 330

uniform mat4 modelMatrix = mat4(1.0);

uniform sampler2D rayEntryTex;
uniform sampler3D volume;
uniform float step;

struct Light
{
    vec3 position;
    vec4 color;
};
uniform Light light;
uniform vec3 viewer;

uniform int mode; // 0: iso, 1: maximum
uniform float iso;

in vec3 fs_rayExit;
out vec4 out_color;

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

vec3 computePhongShading(vec3 pos, vec3 normal)
{
    vec3 worldPos = (modelMatrix * vec4(pos - vec3(0.5), 1.0)).xyz;
    vec3 worldNormal = normalize((modelMatrix * vec4(normal, 0.0)).xyz);
    vec3 toViewer = normalize(viewer - worldPos);
    vec3 toLight = normalize(light.position - worldPos);
    float distance = length(toLight);
    vec3 halfNormal = (toViewer + toLight)/2.0;
    float HdotN = clamp(dot(halfNormal, worldNormal), 0, 1);

    vec3 ambient = vec3(0.05, 0.05, 0.05) * light.color.rgb;
    vec3 diffuse = light.color.rgb * light.color.a * dot(toLight, worldNormal)/(distance*distance);
    vec3 specular = light.color.rgb * light.color.a * pow(HdotN, 100);

    return ambient + diffuse + specular;
}

vec3 computeNormal(vec3 pos)
{
    vec4 offset = vec4(vec3(step), 0);
    float dx = texture(volume, pos.rgb - offset.xww).r - texture(volume, pos.rgb + offset.xww).r;
    float dy = texture(volume, pos.rgb - offset.wyw).r - texture(volume, pos.rgb + offset.wyw).r;
    float dz = texture(volume, pos.rgb - offset.wwz).r - texture(volume, pos.rgb + offset.wwz).r;
    return normalize(vec3(dx/2, dy/2, dz/2));
}

bool computeShadow(vec3 pos)
{
    // perform iso raycasting from position to light
    vec3 lightWorld = (inverse(modelMatrix)*vec4(light.position, 1.0)).xyz;
    Ray ray;
    ray.position = pos;
    ray.direction = normalize(lightWorld - pos);
    ray.terminate = false;

    float shadowStep = 2*step; // no accurate hit required
    // start one step away from the already hit iso surface
    ray.position += shadowStep*ray.direction;

    for(int i=0; i<4096; ++i)
    {
        // FIXME: because of floating point errors, the first step is sometimes skipped
        if(i > 0 && (any(lessThan(ray.position, vec3(0.0))) || any(greaterThan(ray.position, vec3(1.0)))))
            return false;

        float value = texture(volume, ray.position).r;
        if(value >= iso)
            return true;

        ray.position += shadowStep*ray.direction;
    }
    out_color = ray.color;
}

Ray isoSurfaceStep(float value, Ray ray)
{
    if(value >= iso)
    {
        int refinementDir = -1;
        for(int i=1; i<6; ++i)
        {
            ray.position += refinementDir*step/(pow(2, i)) * ray.direction;
            float refinedValue = texture(volume, ray.position.rgb).r;
            if(refinedValue > iso)
                refinementDir = -1;
            else if(refinedValue < iso)
                refinementDir = 1;
            else
                break;
        }

        vec3 normal = computeNormal(ray.position);
        ray.color = vec4(computePhongShading(ray.position, normal), 1.0);
        if(computeShadow(ray.position))
            ray.color.xyz *= 0.1;
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

    for(int i=0; i<4096; ++i)
    {
        // FIXME: because of floating point errors, the first step is sometimes skipped
        if(i > 0 && (any(lessThan(ray.position, vec3(0.0))) || any(greaterThan(ray.position, vec3(1.0)))))
            break;

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
