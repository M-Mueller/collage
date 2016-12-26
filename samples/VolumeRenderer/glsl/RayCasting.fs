#version 330

uniform sampler2D rayEntryTex;
uniform sampler2D rayExitTex;
uniform sampler3D volume;
uniform vec3 volumeSize;
uniform vec3 volumeSpacing;
uniform float step;

uniform float windowWidth;
uniform float windowCenter;

struct Light
{
    vec3 position;
    vec4 color;
};
uniform Light light;
uniform vec3 viewer;
uniform bool lightEnabled;

uniform int mode; // 0: iso, 1: maximum, 2: DVR
uniform float iso;
uniform sampler1D transferFunction;

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

vec3 worldToVoxel(vec3 world)
{
    return (world/(0.5*volumeSize*volumeSpacing) + vec3(1.0))/2.0;
}

float getValue(vec3 position)
{
    vec3 texCoord = worldToVoxel(position);
    if(any(lessThan(texCoord, vec3(0.0))) || any(greaterThan(texCoord, vec3(1.0))))
        return 0.0;
    float value = texture(volume, texCoord).r;
    float left = (windowCenter - 0.5*windowWidth);
    float right = (windowCenter + 0.5*windowWidth);
    value = (value - left)/(right - left);
    return clamp(value, 0.0, 1.0);
}

vec3 computePhongShading(vec3 pos, vec3 normal, vec3 color)
{
    vec3 toViewer = normalize(viewer - pos);
    vec3 toLight = normalize(light.position - pos);
    float distance = length(toLight);
    vec3 halfNormal = (toViewer + toLight)/2.0;
    float HdotN = clamp(dot(halfNormal, normal), 0, 1);

    vec3 ambient = vec3(0.1, 0.1, 0.1) * light.color.rgb;
    vec3 diffuse = light.color.rgb * light.color.a * dot(toLight, normal)/(distance*distance);
    vec3 specular = light.color.rgb * light.color.a * pow(HdotN, 100);

    return color*ambient + color*diffuse + color*specular;
}

vec3 computeNormal(vec3 pos)
{
    vec4 offset = vec4(vec3(step), 0);
    float dx = texture(volume, worldToVoxel(pos.rgb - offset.xww)).r - texture(volume, worldToVoxel(pos.rgb + offset.xww)).r;
    float dy = texture(volume, worldToVoxel(pos.rgb - offset.wyw)).r - texture(volume, worldToVoxel(pos.rgb + offset.wyw)).r;
    float dz = texture(volume, worldToVoxel(pos.rgb - offset.wwz)).r - texture(volume, worldToVoxel(pos.rgb + offset.wwz)).r;
    vec3 normal = vec3(dx/2, dy/2, dz/2);
    float norm = length(normal);
    if(norm != 0.0)
        return normal/norm;
    else
        return vec3(0.0);
}

bool computeShadow(vec3 pos)
{
    // perform iso raycasting from position to light
    Ray ray;
    ray.position = pos;
    ray.direction = normalize(light.position - pos);
    ray.terminate = false;

    // start one step away from the already hit iso surface
    ray.position += step*ray.direction;

    int numSteps = int(length(light.position - pos)/step);
    for(int i=0; i<numSteps; ++i)
    {
        vec3 voxelPos = worldToVoxel(ray.position);
        if(any(lessThan(voxelPos, vec3(0.0))) || any(greaterThan(voxelPos, vec3(1.0))))
            return false;

        float value = getValue(ray.position);
        if(value >= iso)
            return true;

        ray.position += step*ray.direction;
    }
    return false;
}

Ray isoSurfaceStep(float value, Ray ray)
{
    if(value >= iso)
    {
        int refinementDir = -1;
        for(int i=1; i<6; ++i)
        {
            ray.position += refinementDir*step/(pow(2, i)) * ray.direction;
            float refinedValue = getValue(ray.position);
            if(refinedValue > iso)
                refinementDir = -1;
            else if(refinedValue < iso)
                refinementDir = 1;
            else
                break;
        }

        vec3 normal = computeNormal(ray.position);
        ray.color.rgb = computePhongShading(ray.position, normal, vec3(1.0));
        if(computeShadow(ray.position))
            ray.color.rgb *= 0.1;
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

Ray directVolumeRenderingStep(float value, Ray ray)
{
    vec4 color = texture(transferFunction, value);
    if(lightEnabled)
    {
        vec3 normal = computeNormal(ray.position);
        color.rgb = computePhongShading(ray.position, normal, color.rgb);
    }

    ray.color.rgb = ray.color.rgb + (1 - ray.color.a)*color.rgb*color.a;
    ray.color.a = ray.color.a + (1 - ray.color.a)*color.a;

    if(ray.color.a >= 1.0)
        ray.terminate = true;
    return ray;
}

void main()
{
    vec2 texCoord = gl_FragCoord.xy/textureSize(rayEntryTex, 0);
    vec4 rayEnter = texture(rayEntryTex, texCoord);
    vec4 rayExit = texture(rayExitTex, texCoord);

    if(rayExit.a == 0.0 || rayEnter.a == 0.0)
    {
        // ignore pixel outside the proxy geometry
        out_color = vec4(0.0);
        return;
    }

    if(mode == 3)
    {
        out_color = vec4(worldToVoxel(rayEnter.xyz), 1.0);
        return;
    }
    if(mode == 4)
    {
        out_color = vec4(worldToVoxel(rayExit.xyz), 1.0);
        return;
    }

    Ray ray;
    ray.position = rayEnter.xyz;
    ray.direction = normalize(rayExit.xyz - rayEnter.xyz);
    ray.color = vec4(0.0);
    ray.terminate = false;
    ray.maximum = 0.0;

    int numSteps = int(length(rayExit - rayEnter)/step);
    for(int i=0; i<numSteps; ++i)
    {
        float value = getValue(ray.position);
        if(mode == 0)
            ray = isoSurfaceStep(value, ray);
        else if(mode == 1)
            ray = maximumIntensityStep(value, ray);
        else if(mode == 2)
            ray = directVolumeRenderingStep(value, ray);

        if(ray.terminate)
            break;

        ray.position += step*ray.direction;
    }
    out_color = clamp(ray.color, vec4(0.0), vec4(1.0));
}
