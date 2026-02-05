#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D ourTexture1;
uniform vec3 ambient;
uniform vec3 object_color;
uniform bool useTexture;
uniform bool lightObject;

void main()
{
    vec3 amb2 = ambient;
    if (lightObject) amb2 = vec3(1.0f, 1.0f, 1.0f);
    if (useTexture)
    FragColor = vec4((amb2 * object_color) , 1.0f) * texture(ourTexture1, TexCoord);
    else
    FragColor = vec4((amb2 * object_color) , 1.0f);
}
