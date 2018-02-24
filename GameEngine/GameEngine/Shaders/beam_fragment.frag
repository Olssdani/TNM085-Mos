#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
uniform vec3 cameraPos;

void main()
{    
	/*// Inställningar
	vec3 lightColor = vec3(1.0,1.0,1.0);
	vec3 lightPos = vec3(0.0, 0.5,0.5);
	float specularStrength = 5.0;

	//Ambient 
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	
    //Diffuse
    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - Position); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


	//Specular
	vec3 viewPos = cameraPos;
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 50);
	vec3 specular = specularStrength * spec * lightColor;  

	
	
	vec3 result = (ambient+diffuse+specular) *vec3(1.0,1.0,1.0);
	FragColor = vec4(result, 0.5);*/
	
	FragColor = vec4(1.0,1.0,1.0, 1.0);
}