#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material
{
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct Light
{
   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

//uniform sampler2D texture0;
//uniform sampler2D texture1;

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

void main()
{
   // ambient color
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
   
   // diffuse color
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diffuse = max(dot(norm, lightDir), 0.0);
   vec3 diff = diffuse * light.diffuse * vec3(texture(material.diffuse, TexCoord));

   // specular color
   vec3 viewDir = normalize(cameraPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 spec = specular * light.specular * vec3(texture(material.specular, TexCoord));
   
   vec3 color = (ambient + diff + spec);
   
   FragColor = vec4(color, 1.0);
}