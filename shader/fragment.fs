#version 330 core

struct material
{
    vec3 ambient;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform sampler2D ourTexture1;
uniform vec3 ambient;
uniform vec3 object_color;

uniform bool flatShade;
uniform bool useTexture;
uniform bool lightObject;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 camPos;
uniform float specularStrength;
uniform float specularExponent;

uniform material matrl;

void main()
{
    if(flatShade && !lightObject)
    {
        if(useTexture) 
        {
            vec3 texColor = texture(ourTexture1, TexCoord).rgb;
            vec3 result = texColor;
            result *= object_color;
            FragColor = vec4(result, 1.0);
        }
        else if(!useTexture)
        {
            FragColor = vec4(object_color, 1.0);
        }
    }
    else if(!lightObject)
    {
    // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        vec3 viewDir = normalize(camPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
        vec3 specular = specularStrength * spec * lightColor;
        
        if(useTexture) 
        {
            vec3 texColor = texture(ourTexture1, TexCoord).rgb;
            vec3 ambientResult = ambient * texColor;
            vec3 diffuseResult = diffuse * texColor;
            vec3 result = ambientResult + diffuseResult;
            result *= object_color;
            result += specular;
            FragColor = vec4(result, 1.0);
        }
        else if(!useTexture)
        {
            vec3 result = (ambient + diffuse + specular) * object_color;
            FragColor = vec4(result, 1.0);
        }
        return;
    }
    else if (lightObject) FragColor = vec4(object_color, 1.0);
}
