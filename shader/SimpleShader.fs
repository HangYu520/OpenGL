#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;

// Main
void main()
{
   FragColor = vec4(vec3(texture(texture1, TexCoord)), 1.0);
}