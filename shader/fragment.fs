#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform vec3 ambient;
uniform vec3 object_color;

void main()
{
    FragColor = vec4((ambient * object_color) , 1.0f) * texture(ourTexture1, TexCoord);
};
