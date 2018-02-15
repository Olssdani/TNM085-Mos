#version 330 core
layout (location = 0) in vec3 aPos;
<<<<<<< HEAD
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
=======
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;
>>>>>>> 346e6e62f540693273f4151efc593804bdf8f0b4

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
<<<<<<< HEAD
uniform float type_in;

out float type;
out vec3 ourColor;
out vec2 TexCoord;


void main()
{
   gl_Position  = projection * view * model * vec4(aPos, 1.0);
   ourColor = aColor;
   TexCoord = aTexCoord;
   type = type_in;
}
=======

void main()
{
    //Normal = mat3(transpose(inverse(model))) * aNormal;
	Normal = mat3(model)*aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  
>>>>>>> 346e6e62f540693273f4151efc593804bdf8f0b4
