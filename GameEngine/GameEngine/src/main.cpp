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

//SKärmstorlek
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//Center på skärmern
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2;
//Camera värden för FPP kameran
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
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
	Shader ourShader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	Shader skyboxShader("Shaders/skybox_vertex.glsl", "Shaders/skybox_fragment.glsl");
	
	//Våra vertrices för prismat
	float prism[] = {
		// positions				// Normal			/texture
		0.0f,  0.4f, 0.34642,		0.0f, 1.0f, 0.0f,	//1
		0.0f,  0.4f, 0.34642,		0.1890f, 0.0f, -0.9820f,
		0.0f,  0.4f, 0.34642,		-0.1890f, 0.0f, -0.9820f,
		
		0.3f , 0.4f, -0.1732f,		0.0f, 1.0f, 0.0f,	//2
		0.3f,  0.4f, -0.1732f,		0.0f, 0.0f, 1.0f,
		0.3f,  0.4f, -0.1732f,		0.1890f, 0.0f, -0.9820f,
		
		-0.3f, 0.4f, -0.1732f,		0.0f, 1.0f, 0.0f,		//3
		-0.3f, 0.4f, -0.1732f,		0.0f, 0.0f, 1.0f,
		-0.3f, 0.4f, -0.1732f,		-0.1890f, 0.0f, -0.9820f,
		
		0.0f,  -0.4f, 0.34642f,		0.0f, -1.0f, 0.0f,			//4
		0.0f,  -0.4f, 0.34642f,		0.1890, 0.0f, -0.9820f,
		0.0f,  -0.4f, 0.34642f,		-0.1890f, 0.0f, -0.9820f,

		0.3f,  -0.4f, -0.1732f,		0.0f, -1.0f, 0.0f,			//5
		0.3f,  -0.4f, -0.1732f,		0.0f, 0.0f, 1.0f,
		0.3f,  -0.4f, -0.1732f,		0.1890f / 10.0f, 0.0f, -0.9820f,

		-0.3f, -0.4f, -0.1732f,		0.0f, -1.0f, 0.0f,			//6
		-0.3f, -0.4f, -0.1732f,		0.0f, 0.0f, 1.0f,
		-0.3f, -0.4f, -0.1732f,		-0.1890f, 0.0f, -0.9820f,
 	};
	//Vår indices som specifikserar i vilken ordning trianglarna målas upp
	unsigned int prism_ind[] = {
		0, 6, 3,//Top
		9,12,15, //ner
		4,7,13, //mot 1
		7,16,13,//mot2
		2,11,8,// vänster1
		8,11,17,//vänster 2
		1,5,10,//Höger 1
		5,14,10//höger 2
	};

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




	// Våra olika bufferar. Dessa gör så vi kan skicka stora delar vertiser samtidigt så vi slipper skicka 1 i taget
	//VBO(vertex buffer object) skickar våra vertriser till GPU'n
	//Alla buffrar måste vara unsigned ints
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	
	//Skapar ett eller flera buffer objekt
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binder Buffern till det sóm den skall bindas till
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(prism), prism, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prism_ind), prism_ind, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
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




	// Gör så vi har en depth buffer, dvs en z buffer så opengl vet vad som ligger bakom och framför 
	glEnable(GL_DEPTH_TEST);

	
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	std::vector<std::string> faces
	{
		"wall.jpg",
		"wall2.jpg",
		"floor.jpg",
		"floor.jpg",
		"side.jpg",
		"wall3.jpg"
	};

	//initering av kuben
	unsigned int cubemapTexture = loadCubemap(faces);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	
	//Modell och view matris
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.2f));
	//Projektions matris
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		// Beräknar fram tiden sen sist gång vi loppade
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// Tar bort allt från skärmen och reseten.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourShader.use();

		//transform

		model = glm::rotate(model, deltaTime * glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(glGetUniformLocation(ourShader.ID, "cameraPos"), 1, &cameraPos[0]);
		glUniform1f(glGetUniformLocation(ourShader.ID, "type_in"), 0.0f);
		glBindVertexArray(VAO);		
		glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, 0);
		
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
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
	float cameraSpeed = 1.0f * deltaTime;
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