#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Data Structures
struct Color
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct Material
{
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct DirLight 
{
   vec3 direction;
   Color lightColor;
};

struct PointLight 
{
   vec3 position;
   float constant;
   float linear;
   float quadratic;
   Color lightColor;  
};

struct SpotLight 
{
   vec3 position;
   vec3 direction;
   float cutOff;
   float outerCutOff;
   Color lightColor;
};

uniform Material material;
uniform vec3 cameraPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform int lightMode;

// Function
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, Color materialColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Color materialColor); 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Color materialColor);
Color CalPhongColor(vec3 lightDir, vec3 normal, vec3 viewDir, Color materialColor, Color lightColor);

// Main
void main()
{
   vec3 norm = normalize(Normal);

   vec3 viewDir = normalize(cameraPos - FragPos);

   Color materialColor = {
      vec3(texture(material.diffuse, TexCoord)),
      vec3(texture(material.diffuse, TexCoord)),
      vec3(texture(material.specular, TexCoord))
   };

   vec3 color;

   if (lightMode == 1)
   {
      color = CalcDirLight(dirLight, norm, viewDir, materialColor);
   }
   else if (lightMode == 2)
   {
      color = CalcPointLight(pointLight, norm, FragPos, viewDir, materialColor);
   }
   else if (lightMode == 3)
   {
      color = CalcSpotLight(spotLight, norm, FragPos, viewDir, materialColor);
   }
   
   FragColor = vec4(color, 1.0);
}

// Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, Color materialColor)
{
   vec3 lightDir = normalize(-light.direction);
   Color color = CalPhongColor(lightDir, normal, viewDir, materialColor, light.lightColor);
   return (color.ambient + color.diffuse + color.specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Color materialColor)
{
   vec3 lightDir = normalize(light.position - fragPos);
   Color color = CalPhongColor(lightDir, normal, viewDir, materialColor, light.lightColor);
   
   float distance    = length(light.position - fragPos);// attenuation
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   
   color.ambient  *= attenuation;
   color.diffuse  *= attenuation;
   color.specular *= attenuation;
   return (color.ambient + color.diffuse + color.specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Color materialColor)
{
   vec3 lightDir = normalize(light.position - fragPos);
   
   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

   Color color;
   if (theta > light.outerCutOff)
   {
      color = CalPhongColor(lightDir, normal, viewDir, materialColor, light.lightColor);
      color.diffuse *= intensity;
      color.specular *= intensity;
   }
   else
   {
      color.ambient = materialColor.ambient * light.lightColor.ambient;
      color.diffuse = vec3(0.0);
      color.specular = vec3(0.0);
   }
   return (color.ambient + color.diffuse + color.specular);
}

Color CalPhongColor(vec3 lightDir, vec3 normal, vec3 viewDir, Color materialColor, Color lightColor)
{
   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);
   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // combine results
   Color color;
   color.ambient = materialColor.ambient * lightColor.ambient;
   color.diffuse = materialColor.diffuse * lightColor.diffuse * diff;
   color.specular = materialColor.specular * lightColor.specular * spec;
   return color;
}