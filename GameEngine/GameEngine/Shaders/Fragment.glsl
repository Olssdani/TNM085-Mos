#version 330 core
out vec4 FragColor;

in float type;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;



void main()
{
	if(type == 0.0f){
		FragColor = vec4(ourColor, 1.0f);	
	}else{
	FragColor = vec4(ourColor, 1.0f);
		
		FragColor = texture(ourTexture, TexCoord);
	}
    
}