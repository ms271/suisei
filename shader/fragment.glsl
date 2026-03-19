#version 330 core

struct matrl
{
    sampler2D mainTex;
    vec3 mainVec;

    vec3 ambVec;
    sampler2D diffTex;
    vec3 diffVec;

    sampler2D specTex;
    vec3 specVec;
    float shininess;
};

struct posLgt 
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct dirLgt
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct flashLgt
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float cutOff2;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform vec3 object_color;

uniform bool flatShade;
uniform bool useFlatTex;

uniform bool useDiffTex;
uniform bool useSpecTex;
uniform int useLightType;

uniform vec3 camPos;

uniform float specularStrength;
uniform float specularExponent;

uniform matrl material;
uniform posLgt posLight;
uniform dirLgt dirLight;
uniform flashLgt flashLight;

vec3 CalcDirLight(dirLgt light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(posLgt light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(flashLgt light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    if(!flatShade)
    {
        vec3 diffuse, ambient, specular;

        //Normal, light, FragPos
        vec3 norm = normalize(Normal);

        vec3 viewDir = normalize(camPos - FragPos);
        
        vec3 result = vec3(0);

        if(useLightType == 0)
        {
            result += CalcPointLight(posLight, norm, fragPos, viewDir);
        }
        else if(useLightType == 1)
        {
            result += CalcDirLight(dirLight, norm, viewDir);
        }
        else if(useLightType == 2)
        {
            result += CalcFlashLight(flashLight, norm, fragPos, viewDir);
        }
        float diff = max(dot(norm, lightDir), 0.0);
        
        //material
        vec3 objCol;
        if(useFlatTex) objCol = texture(material.mainTex, TexCoord).rgb;
        else objCol = material.mainVec;

        //TexCoord, useDiffTex
        if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * lightDiff * diffTex;
            ambient = lightAmb * diffTex;
        }
        else
        {
            ambient = lightAmb * objCol * material.ambVec;
            diffuse = diff * lightDiff * objCol * material.diffVec;
        }
        //camPos
        
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        //useSpecTex
        if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * lightSpec;
        else 
            specular = spec * lightSpec * material.specVec;
    
        vec3 result = ambient + diffuse + specular;
        result *= lightAttenuation;
        
        //FragColor
        FragColor = vec4(result, 1.0);
    }
    else
    {
        //object_texture, useTex
        if(!useFlatTex) FragColor = vec4(material.mainVec, 1.0);
        else FragColor = vec4(texture(material.mainTex, TexCoord).rgb, 1.0);
    }
}

vec3 CalcDirLight(dirLgt light, vec3 normal, vec3 viewDir, bool useDiffTex, bool useSpecTex)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * lightDiff * diffTex;
            ambient = lightAmb * diffTex;
        }
        else
        {
            ambient = lightAmb * objCol * material.ambVec;
            diffuse = diff * lightDiff * objCol * material.diffVec;
        }
    if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * lightSpec;
        else 
            specular = spec * lightSpec * material.specVec;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(posLgt light, vec3 normal, vec3 fragPos, vec3 viewDir, bool useDiffTex, bool useSpecTex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * lightDiff * diffTex;
            ambient = lightAmb * diffTex;
        }
        else
        {
            ambient = lightAmb * objCol * material.ambVec;
            diffuse = diff * lightDiff * objCol * material.diffVec;
        }
    if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * lightSpec;
        else 
            specular = spec * lightSpec * material.specVec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcFlashLight(flashLgt light, vec3 normal, vec3 fragPos, vec3 viewDir, bool useDiffTex, bool useSpecTex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 3.5);
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * lightDiff * diffTex;
            ambient = lightAmb * diffTex;
        }
        else
        {
            ambient = lightAmb * objCol * material.ambVec;
            diffuse = diff * lightDiff * objCol * material.diffVec;
        }
    if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * lightSpec;
        else 
            specular = spec * lightSpec * material.specVec;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}