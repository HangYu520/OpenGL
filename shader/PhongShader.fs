#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform sampler2D texture0;
//uniform sampler2D texture1;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform float ambientStrength;
uniform float diffStrength;
uniform float specStrength;

void main()
{
   // ambient color
   vec3 ambient = ambientStrength * lightColor;
   
   // diffuse color
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diffuse = max(dot(norm, lightDir), 0.0);
   vec3 diff = diffStrength * diffuse * lightColor;

   // specular color
   vec3 viewDir = normalize(cameraPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 spec = specStrength * specular * lightColor;
   
   vec3 color = (ambient + diff + spec) * objColor;
   
   FragColor = vec4(color, 1.0);
}