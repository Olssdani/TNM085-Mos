#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
/*void main()
{             
    float ratio = 1.00 / 1.52;
	float Air = 1.00;
	float Glass = 1.52;
	float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));
	vec3 I = normalize(Position - cameraPos);
	vec3 R_refract= refract(I, normalize(Normal), ratio);
	vec3 R_reflect = reflect(I, normalize(Normal));
	//float v_fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-I, Normal)), 5.0);
	//vec4 refractionColor = texture(skybox, normalize(R_refract));
	//vec4 reflectionColor = texture(skybox, normalize(R_reflect));
	
	
	FragColor = texture(skybox, normalize(R_reflect));
} */
void main()
{             
		 vec3 I = normalize(Position-cameraPos);
		 vec3 R = reflect(I, normalize(Normal));

		 float rim = pow(1.0-max(dot(-I, normalize(Normal)), 0.0), 8);

		 FragColor = vec4(texture(skybox, R).rgb * rim, 1.0);

		 // FragColor = vec4(texture(skybox, R).rgb, 1.0);
}

/*
void main()
{             
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
} */ 