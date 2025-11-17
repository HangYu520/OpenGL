#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

//uniform sampler2D texture0;
//uniform sampler2D texture1;
uniform vec3 objColor;
uniform vec3 lightColor;

void main()
{
   FragColor = vec4(objColor * lightColor, 1.0);
}