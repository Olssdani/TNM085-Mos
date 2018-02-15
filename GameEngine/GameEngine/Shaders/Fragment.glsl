
#version 330 core
out vec4 FragColor;
<<<<<<< HEAD

in float type;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

=======
>>>>>>> 346e6e62f540693273f4151efc593804bdf8f0b4

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
void main()
<<<<<<< HEAD
{
	if(type == 0.0f){
		FragColor = vec4(ourColor, 1.0f);	
	}else{
	FragColor = vec4(ourColor, 1.0f);
		
		FragColor = texture(ourTexture, TexCoord);
	}
    
}
=======
{             
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
} 
//void main()
//{             
  //  vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
//}
>>>>>>> 346e6e62f540693273f4151efc593804bdf8f0b4
