#version 450 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 calDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 calPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 calSpotLight(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

const int PL_NUM=4;

in vec3 normal;
in vec3 frag_pos;
in vec2 texture_coords;

out vec4 frag_color;

uniform vec3 view_pos;
uniform Material material;
uniform DirLight dir_light;
uniform PointLight point_light[PL_NUM];
uniform SpotLight spot_light;

void main()
{
    vec3 norm=normalize(normal);
    vec3 view_dir=normalize(view_pos-frag_pos);

    vec3 result=calDirLight(dir_light, norm, view_dir);
    for(int i=0; i<PL_NUM; i++)
        result+=calPointLight(point_light[i], norm, frag_pos, view_dir);
    result+=calSpotLight(spot_light, norm, frag_pos, view_dir);

    frag_color=vec4(result, 1.0);
}

vec3 calDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir=normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 reflect_dir=reflect(-light_dir, normal);
    float spec=pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);

    vec3 ambient=light.ambient*vec3(texture(material.diffuse, texture_coords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse, texture_coords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular, texture_coords));

    return ambient+diffuse+specular;
}

vec3 calPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir=normalize(light.position-frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 reflect_dir=reflect(-light_dir, normal);
    float spec=pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);

    float distance=length(light.position-frag_pos);
    float attenuation=1.0f/(light.constant+light.linear*distance+light.quadratic*(distance*distance));

    vec3 ambient=light.ambient*vec3(texture(material.diffuse, texture_coords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse, texture_coords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular, texture_coords));

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return ambient+diffuse+specular;
}

vec3 calSpotLight(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir=normalize(light.position-frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 reflect_dir=reflect(-light_dir, normal);
    float spec=pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);

    float distance=length(light.position-frag_pos);
    float attenuation=1.0f/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    float theta=dot(light_dir, normalize(-light.direction));
    float epsilon=light.cutoff-light.outer_cutoff;
    float intensity=clamp((theta-light.outer_cutoff)/epsilon, 0.0f, 1.0f);

    vec3 ambient=light.ambient*vec3(texture(material.diffuse, texture_coords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse, texture_coords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular, texture_coords));

    ambient*=attenuation*intensity; 
    diffuse*=attenuation*intensity;
    specular*=attenuation*intensity;

    return ambient+diffuse+specular;
}
