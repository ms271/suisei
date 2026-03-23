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

struct aiMatrl
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
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

#define NR_POS_LIGHTS 4

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 object_color;

uniform bool assimp;

uniform bool flatShade;
uniform bool useFlatTex;

uniform bool useMainTex;
uniform bool useDiffTex;
uniform bool useSpecTex;

uniform vec3 camPos;

uniform float specularStrength;
uniform float specularExponent;

uniform matrl material;
uniform aiMatrl aspMaterial;
uniform posLgt[NR_POS_LIGHTS] posLight;
uniform dirLgt dirLight;
uniform flashLgt flashLight;

vec3 CalcDirLight(dirLgt light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(posLgt light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(flashLgt light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcDirLightAss(dirLgt light, vec3 normal, vec3 viewDir);
vec3 CalcPointLightAss(posLgt light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLightAss(flashLgt light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    if(assimp)
    {
        //Normal, light, FragPos
        vec3 norm = normalize(Normal);

        vec3 viewDir = normalize(camPos - FragPos);
        
        vec3 result = vec3(0);

        for(int i = 0; i < NR_POS_LIGHTS; i++)
        {
            if(posLight[i].constant == 0) continue;

            result += CalcPointLightAss(posLight[i], norm, FragPos, viewDir);
        }

        result += CalcDirLightAss(dirLight, norm, viewDir);
        
        if(flashLight.constant != 0)
        result += CalcFlashLightAss(flashLight, norm, FragPos, viewDir);
        
        //FragColor
        FragColor = vec4(result, 1.0);
    }
    else if(!flatShade)
    {
        //Normal, light, FragPos
        vec3 norm = normalize(Normal);

        vec3 viewDir = normalize(camPos - FragPos);
        
        vec3 result = vec3(0);

        for(int i = 0; i < NR_POS_LIGHTS; i++)
        {
            if(posLight[i].constant == 0) continue;

            result += CalcPointLight(posLight[i], norm, FragPos, viewDir);
        }

        result += CalcDirLight(dirLight, norm, viewDir);
        
        if(flashLight.constant != 0)
        result += CalcFlashLight(flashLight, norm, FragPos, viewDir);
        
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

vec3 CalcDirLight(dirLgt light, vec3 normal, vec3 viewDir)
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

    vec3 objCol;
    if(useMainTex) objCol = objCol = vec3(material.mainVec);
    else objCol = vec3(material.mainVec);

    if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        else
        {
            ambient = light.ambient * objCol * material.ambVec;
            diffuse = diff * light.diffuse * objCol * material.diffVec;
        }
    if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * light.specular;
        else 
            specular = spec * light.specular * material.specVec;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(posLgt light, vec3 normal, vec3 fragPos, vec3 viewDir)
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

    vec3 objCol;
    if(useMainTex) objCol = objCol = vec3(material.mainVec);
    else objCol = vec3(material.mainVec);

    if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        else
        {
            ambient = light.ambient * objCol * material.ambVec;
            diffuse = diff * light.diffuse * objCol * material.diffVec;
        }
    if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * light.specular;
        else 
            specular = spec * light.specular * material.specVec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcFlashLight(flashLgt light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    float epsilon = light.cutOff - light.cutOff2;
    float intensity = clamp((theta - light.cutOff2) / epsilon, 0.0, 3.5);
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 objCol;
    if(useMainTex) objCol = objCol = vec3(material.mainVec);
    else objCol = vec3(material.mainVec);

    if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        else
        {
            ambient = light.ambient * objCol * material.ambVec;
            diffuse = diff * light.diffuse * objCol * material.diffVec;
        }
    if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * light.specular;
        else 
            specular = spec * light.specular * material.specVec;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
//assimp
vec3 CalcDirLightAss(dirLgt light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results

    vec3 ambient = vec3(0);
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    vec3 objCol = vec3(1);
    //if(useMainTex) objCol = objCol = vec3(material.mainVec);
    //else objCol = vec3(material.mainVec);

    bool alp = false;

    if(alp)
        {
            vec3 diffTex = vec3(texture(aspMaterial.texture_diffuse1, TexCoord));
            diffTex += vec3(texture(aspMaterial.texture_diffuse2, TexCoord));
            diffTex += vec3(texture(aspMaterial.texture_diffuse3, TexCoord));

            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        else
        {
            ambient = light.ambient * objCol;
            diffuse = diff * light.diffuse * objCol;
        }
    if(alp)
    {
        specular += vec3(texture(aspMaterial.texture_specular1, TexCoord)) * spec * light.specular;
        specular += vec3(texture(aspMaterial.texture_specular2, TexCoord)) * spec * light.specular;
    }
    else 
            specular = spec * light.specular;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLightAss(posLgt light, vec3 normal, vec3 fragPos, vec3 viewDir)
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

    vec3 objCol = vec3(1);
    //if(useMainTex) objCol = objCol = vec3(material.mainVec);
    //else objCol = vec3(material.mainVec);
    bool alp = false;
    if(alp)
        {
            vec3 diffTex = vec3(texture(aspMaterial.texture_diffuse1, TexCoord));
            //diffTex += vec3(texture(aspMaterial.texture_diffuse2, TexCoord));
            //diffTex += vec3(texture(aspMaterial.texture_diffuse3, TexCoord));

            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        else
        {
            ambient = light.ambient * objCol;
            diffuse = diff * light.diffuse * objCol;
        }
    if(alp)
    {
            specular += vec3(texture(aspMaterial.texture_specular1, TexCoord)) * spec * light.specular;
        //specular += vec3(texture(aspMaterial.texture_specular2, TexCoord)) * spec * light.specular;
    }
        else 
            specular = spec * light.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcFlashLightAss(flashLgt light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    float epsilon = light.cutOff - light.cutOff2;
    float intensity = clamp((theta - light.cutOff2) / epsilon, 0.0, 3.5);
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 objCol = vec3 (1);
    //if(useMainTex) objCol = objCol = vec3(material.mainVec);
    //else objCol = vec3(material.mainVec);

    bool asp = false;

    if(asp)
        {
            vec3 diffTex = vec3(texture(aspMaterial.texture_diffuse1, TexCoord));
            //diffTex += vec3(texture(aspMaterial.texture_diffuse2, TexCoord));
            //diffTex += vec3(texture(aspMaterial.texture_diffuse3, TexCoord));

            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        else
        {
            ambient = light.ambient * objCol * material.ambVec;
            diffuse = diff * light.diffuse * objCol * material.diffVec;
        }
    if(asp)
        {
                specular += vec3(texture(aspMaterial.texture_specular1, TexCoord)) * spec * light.specular;
        //specular += vec3(texture(aspMaterial.texture_specular2, TexCoord)) * spec * light.specular;
}   
else 
            specular = spec * light.specular * material.specVec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}