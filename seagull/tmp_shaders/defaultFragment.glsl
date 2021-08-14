#version 400


// in vec2 TexCoords;
// in vec3 FragPos;
// in vec3 Normal;

// in mat3 TBN;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;


struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    sampler2D texture_ao1;
    sampler2D texture_emissive1;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};

// struct DirLight {
    // vec3 direction;

    // vec3 ambient;
    // vec3 diffuse;
    // vec3 specular;
// };

// uniform DirLight dirLight;


// struct PointLight {
    // vec3 position;

    // float constant;
    // float linear;
    // float quadratic;

    // vec3 ambient;
    // vec3 diffuse;
    // vec3 specular;
// };

uniform Light lighting;

uniform Material material;
// uniform Light light;

// uniform vec3 viewPos;
// uniform vec3 lightPos;


// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
    // vec3 lightDir = normalize(-light.direction);
    // // diffuse shading
    // float diff = max(dot(norm, lightDir), 0.0);
    // // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // // combine results
    // vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    // return (ambient + diffuse + specular);
// }

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(fs_in.TangentLightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    if (diff == 0.0)
        spec = 0.0;


    vec3 ao = texture(material.texture_ao1, fs_in.TexCoords).rgb;
    vec3 emissive = texture(material.texture_emissive1, fs_in.TexCoords).rgb;

    // attenuation
    float distance    = length(fs_in.TangentLightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse1, fs_in.TexCoords).rgb * ao;
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoords).rgb;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (emissive + (ambient + diffuse + specular ));
}

void main(void)
{
    // properties
    vec3 norm = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec3 ao = texture(material.texture_ao1, fs_in.TexCoords).rgb;
    vec3 emissive = texture(material.texture_emissive1, fs_in.TexCoords).rgb;

    vec3 result = CalcPointLight(lighting, norm, fs_in.TangentFragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
