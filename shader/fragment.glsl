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

struct lgt 
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform vec3 object_color;

uniform bool flatShade;
uniform sampler2D object_texture;
uniform bool useFlatTex;

uniform bool useDiffTex;
uniform bool useSpecTex;
uniform bool lightObject;

uniform vec3 camPos;

uniform float specularStrength;
uniform float specularExponent;

uniform matrl material;
uniform lgt light;

void main()
{
    if(!flatShade)
    {
        vec3 diffuse, ambient, specular;

        //Normal, light, FragPos
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
    
        float diff = max(dot(norm, lightDir), 0.0);
        
        vec3 objCol;
        if(useFlatTex) objCol = texture(material.mainTex, TexCoord).rgb;
        else objCol = material.mainVec;

        //material, TexCoord, useDiffTex
        if(useDiffTex)
        {
            vec3 diffTex = vec3(texture(material.diffTex, TexCoord));
            diffuse = diff * light.diffuse * diffTex;
            ambient = light.ambient * diffTex;
        }
        //object_color
        else
        {
            ambient = light.ambient * objCol * material.ambVec;
            diffuse = diff * light.diffuse * objCol * material.diffVec;
        }
        //camPos
        vec3 viewDir = normalize(camPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        //useSpecTex
        if(useSpecTex)
            specular = vec3(texture(material.specTex, TexCoord)) * spec * light.specular;
        else 
            specular = spec * light.specular * material.specVec;
    
        vec3 result = ambient + diffuse + specular;
        //FragColor
        FragColor = vec4(result, 1.0);
    }
    else
    {
        //object_texture, useTex
        if(!useFlatTex) FragColor = vec4(object_color, 1.0);
        else vec3 FragColor = texture(object_texture, TexCoord).rgb;
    }
}