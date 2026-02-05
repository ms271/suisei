#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform sampler2D ourTexture1;
uniform vec3 ambient;
uniform vec3 object_color;
uniform bool useTexture;
uniform bool lightObject;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 camPos;
uniform float specularStrength;
uniform float specularExponent;

void main()
{
    if(!lightObject)
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
            vec3 result = ambientResult + diffuseResult + specular;
            result *= object_color;
            FragColor = vec4(result, 1.0);
        }
        else
        {
            vec3 result = (ambient + diffuse + specular) * object_color;
            FragColor = vec4(result, 1.0);
        }
    }
    else FragColor = vec4(object_color, 1.0);
}
