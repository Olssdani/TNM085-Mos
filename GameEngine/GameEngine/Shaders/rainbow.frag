#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 Coords;
//uniform vec3 cameraPos;

//uniform sampler2D ourTexture;

void main()
{    
	float value = Position.z*10;
	//FragColor = vec4(Normal,pow(0.9,pow(2,abs(value))));
	FragColor = vec4(Normal,min(1-pow(Position.x,2),0.8f));
	//FragColor = texture(ourTexture, Coords);
}