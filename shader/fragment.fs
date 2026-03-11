#version 330 core

struct matrl
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

uniform sampler2D ourTexture1;
uniform vec3 object_color;

uniform bool flatShade;
uniform bool useTexture;
uniform bool lightObject;

uniform vec3 camPos;
uniform float specularStrength;
uniform float specularExponent;

uniform matrl material;
uniform lgt light;

void main()
{
    if(flatShade && !lightObject)
    {
        if(useTexture) 
        {
        //in texColor, object_color
            vec3 texColor = texture(ourTexture1, TexCoord).rgb;
            vec3 result = texColor;
            result *= object_color;
            FragColor = vec4(result, 1.0);
        }
        else if(!useTexture)
        {
        //in object_color
            FragColor = vec4(object_color, 1.0);
        }
    }
    else if(!lightObject)
    {
    // ambient in linght, material
        vec3 ambient = light.ambient * material.ambient;
    // diffuse in Normal, light, material, FragPos
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (diff * material.diffuse) * light.ambient;
    // specular in camPos, FragPos, material, light
        vec3 viewDir = normalize(camPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = material.specular * spec * light.specular;
        
        if(useTexture) 
        {
            // in ourTexture1, TexCoord
            vec3 texColor = texture(ourTexture1, TexCoord).rgb;
            vec3 ambientResult = ambient * texColor;
            vec3 diffuseResult = diffuse * texColor;
            vec3 result = ambientResult + diffuseResult + specular;
            FragColor = vec4(result, 1.0);
        }
        else if(!useTexture)
        {
            vec3 result = (ambient + diffuse + specular);
            FragColor = vec4(result, 1.0);
        }
        return;
    }
    else if (lightObject) FragColor = vec4(object_color, 1.0);
}
