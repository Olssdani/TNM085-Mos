#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;



void main()
{    
	float value = Position.z*10;
	//FragColor = vec4(Normal,pow(0.9,pow(2,abs(value))));
	FragColor = vec4(Normal,min(1-pow(Position.x,2),0.8f));
}