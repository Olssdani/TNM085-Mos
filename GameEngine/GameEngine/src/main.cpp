#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "shader.h"
#include <string>
#include <vector>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Funktion Def
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int loadTexture(char const * path);
void updateNormals(float vert[], const unsigned int ind[], const unsigned int length);

//SKärmstorlek
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Center på skärmern
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2;

//Camera värden för FPP kameran
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

// GLobala variabler för cameran
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.9f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//GLobal projectionmatris
glm::mat4 projection = glm::mat4(1.0f);

//Globala tidsvariabler
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
	//Startar upp glfw som är ett wrapper biblotek för opengl som hanterar fönster och inputs
	glfwInit();
	// Sätter vilken opengl version vi vill använda. Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Avaktiverar gammal funktionalitet så vi får tillgång till ny
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Denna raden behövs för mac!!!!!!!!!!!!!!
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	//Skapar pointer till fönstret objektet
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Prisma", glfwGetPrimaryMonitor(), NULL); //Fullscreen men blev fel med viewport, har inte orkat lösa de än
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Prisma", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Sätter vårt fönster till main fönstret ifall vi skulle ha flera fönster
	glfwMakeContextCurrent(window);
	// Callback funktion som kallas så får vi uppdaterar skärmstorleken och sedan ändrar viewporten
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Callback funktion som kallas så fort musen rör på sig 
	glfwSetCursorPosCallback(window, mouse_callback);

	//Gör massa saker så att vi slipper det. Typ att plocka fram information om vårt grafikkort
	//Fråga inte mig hur de funkar!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// Här byggs vår shaders
	Shader prismShader("Shaders/Vertex.vert", "Shaders/Fragment.frag");
	Shader skyboxShader("Shaders/skybox_vertex.vert", "Shaders/skybox_fragment.frag");
	Shader beamShader("Shaders/beam_vertex.vert", "Shaders/beam_fragment.frag");
	//Våra vertrices för prismat
	float prism[] = {
		// positions				// Normal			/texture
		0.0f,  0.4f, 0.34642,		0.0f, 0.0f, 0.0f,	//1		//0
		0.0f,  0.4f, 0.34642,		0.0f, 0.0f, 0.0f,			//1
		0.0f,  0.4f, 0.34642,		0.0f, 0.0f, 0.0f,			//2

		0.3f , 0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,	//2		//3
		0.3f,  0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//4
		0.3f,  0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//5

		-0.3f, 0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,	//3		//6
		-0.3f, 0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//7
		-0.3f, 0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//8

		0.0f,  -0.4f, 0.34642f,		0.0f, 0.0f, 0.0f,	//4		//9
		0.0f,  -0.4f, 0.34642f,		0.0, 0.0f, 0.0f,			//10
		0.0f,  -0.4f, 0.34642f,		0.0f, 0.0f, 0.0f,			//11

		0.3f,  -0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,	//5		//12
		0.3f,  -0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//13
		0.3f,  -0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//14

		-0.3f, -0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,	//6		//15
		-0.3f, -0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//16
		-0.3f, -0.4f, -0.1732f,		0.0f, 0.0f, 0.0f,			//17
	};
	//Vår indices som specifikserar i vilken ordning trianglarna målas upp
	unsigned int prism_ind[24] = {
		0, 3, 6,//Top
		9,15,12, //ner
		1,10,14, //1
		1,14,5,//2
		2,17,11,// 3
		2,8,17,//4
		4,13,16,//5
		4,16,7//6
	};
	
	//Uppdaterar normalerna
	updateNormals(prism, prism_ind, sizeof(prism_ind) / sizeof(prism_ind[0]));

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	float beam[] = {
	0.01f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,
	0.01f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,

	0.01f, 0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	0.01f, 0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

	-0.01f, 0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	-0.01f, 0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

	-0.01f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f
	-0.01f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f
	};

	unsigned int beam_ind[]{
	0,2,4,
	0,4,6


	};

	updateNormals(beam, beam_ind, sizeof(beam_ind) / sizeof(beam_ind[0]));


	// Våra olika bufferar. Dessa gör så vi kan skicka stora delar vertiser samtidigt så vi slipper skicka 1 i taget
	//VBO(vertex buffer object) skickar våra vertriser till GPU'n
	//Alla buffrar måste vara unsigned ints
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	
	//Skapar ett eller flera buffer objekt
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binder Buffern till det sóm den skall bindas till. I detta fall prismat
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(prism), prism, GL_STATIC_DRAW);
	//Knyter indeices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prism_ind), prism_ind, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// skybox
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//bEAM
	unsigned int beamVBO, beamVAO, beamEBO;
	glGenVertexArrays(1, &beamVAO);

	//Skapar ett eller flera buffer objekt
	glGenBuffers(1, &beamVBO);
	glGenBuffers(1, &beamEBO);

	glBindVertexArray(beamVAO);

	//Binder Buffern till det sóm den skall bindas till. I detta fall prismat
	glBindBuffer(GL_ARRAY_BUFFER, beamVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(beam), beam, GL_STATIC_DRAW);
	//Knyter indeices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, beamEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(beam_ind), beam_ind, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Gör så vi har en depth buffer, dvs en z buffer så opengl vet vad som ligger bakom och framför 
	glEnable(GL_DEPTH_TEST);

	//Ifall vi vill rendera med endast linjerna
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//Tar bort musen från skärmen 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Namn på våra bilder för kuben
	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	//initering av kuben
	unsigned int cubemapTexture = loadCubemap(faces);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	
	//Modell och view matris
	glm::mat4 model = glm::mat4(1.0f);//Rotationsmatrisen för prismat
	glm::mat4 view = glm::mat4(1.0f); // Vy matrisen
	model = glm::scale(model, glm::vec3(0.1f));
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);//Projektionsmatrisen

	// Sätter att bara framsidan syns
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);
	
	while (!glfwWindowShouldClose(window))
	{
		//Knyter om våra shader vilket betyder att vi kan ändra i shaderna i realtid
		Shader prismShader("Shaders/Vertex.vert", "Shaders/Fragment.frag");
		Shader skyboxShader("Shaders/skybox_vertex.vert", "Shaders/skybox_fragment.frag");
		Shader beamShader("Shaders/beam_vertex.vert", "Shaders/beam_fragment.frag");
		// Beräknar fram tiden sen sist gång vi loppade
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Kollar input
		processInput(window);

		// Tar bort allt från skärmen.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rotation av prismat
		//model = glm::rotate(model, deltaTime * glm::pi<float>() / 10, glm::vec3(0.0f, 1.0f, 0.0f));
		
		//Updaterar vy matrisen
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//Startar prismat shadern
		prismShader.use();
		//Skickar en massa skit till shadern
		glUniformMatrix4fv(glGetUniformLocation(prismShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(prismShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(prismShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(glGetUniformLocation(prismShader.ID, "cameraPos"), 1, &cameraPos[0]);
		//Binder vertexen och sedan ritar dem
		glBindVertexArray(VAO);		
		glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, 0);

		// beam
	
		beamShader.use();
		glUniformMatrix4fv(glGetUniformLocation(beamShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(beamShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		// skybox cube
		glBindVertexArray(beamVAO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Återställas z buffer typ
		glDepthFunc(GL_LESS); // set depth function back to default

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.1f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	
	glViewport(0, 0, width, height);

	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

//Uppdaterar och 
void updateNormals(float vert [], const unsigned int ind[], const unsigned int length) 
{
	glm::vec3 temp =glm::vec3(1.0f);
	for (int i = 0; i < length; i = i + 3) {
		glm::vec3 first = glm::vec3(vert[ind[i + 1] * 6] - vert[ind[i] * 6], vert[ind[i + 1] * 6 + 1] - vert[ind[i] * 6 + 1], vert[ind[i + 1] * 6 + 2] - vert[ind[i] * 6 + 2]);
		glm::vec3 second = glm::vec3(vert[ind[i + 2] * 6] - vert[ind[i + 1] * 6], vert[ind[i + 2] * 6 + 1] - vert[ind[i + 1] * 6 + 1], vert[ind[i + 2] * 6 + 2] - vert[ind[i + 1] * 6 + 2]);
		temp = cross(first, second);

		std::cout << temp[0] << " " << temp[1] << " " << temp[2] << std::endl;
		vert[ind[i] * 6 + 3] = temp[0];
		vert[ind[i] * 6 + 4] = temp[1];
		vert[ind[i] * 6 + 5] = temp[2];

		vert[ind[i + 1] * 6 + 3] = temp[0];
		vert[ind[i + 1] * 6 + 4] = temp[1];
		vert[ind[i + 1] * 6 + 5] = temp[2];

		vert[ind[i + 2] * 6 + 3] = temp[0];
		vert[ind[i + 2] * 6 + 4] = temp[1];
		vert[ind[i + 2] * 6 + 5] = temp[2];
	}
}