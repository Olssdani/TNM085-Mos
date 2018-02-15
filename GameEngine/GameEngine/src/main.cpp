#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "shader.h"
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
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
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
	//Våra vertrices för prismat
	float prism[] = {
		// positions          // colors          
		0.0f,  0.4f, 0.34642,   1.0f, 0.0f, 0.0f,   0.0f,0.0f,
		0.3f,  0.4f, -0.1732f,  1.0f, 0.0f, 0.0f,   0.0f,0.0f,
		-0.3f,  0.4f,-0.1732f,   1.0f, 0.0f, 0.0f,   0.0f,0.0f,
		0.0f,   -0.4f,0.34642,   0.0f, 1.0f, 0.0f,  0.0f,0.0f,
		0.3f,  -0.4f,-0.1732f,   0.0f, 1.0f, 0.0f,  0.0f,0.0f,
		-0.3f, -0.4f, -0.1732f,   0.0f, 1.0f, 0.0f,  0.0f,0.0f
 	};
	//Vår indices som specifikserar i vilken ordning trianglarna målas upp
	unsigned int prism_ind[] = {
		0, 2, 1,
		3,5,4,
		0,1,3,
		1,4,3,
		0,3,2,
		2,3,5,
		2,5,1,
		1,5,4
	};

	float Wall[] = {
		// positions          // colors     
		-1.0f,  -1.0f, 1.0,     1.0f, 0.0f, 0.0f,  0.0f,0.0f,
		1.0f,  -1.0f, 1.0f,     0.0f, 1.0f, 0.0f,  1.0f,0.0f,
		1.0f,  -1.0f, -1.0f,    0.0f, 0.0f, 1.0f,  1.0f,1.0f,
		-1.0f,  -1.0f, -1.0f,   1.0f, 0.0f, 0.0,   0.0f,1.0f,
		-1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,  0.0f,0.0f,
		1.0f,  1.0f, -1.0f,     1.0f, 0.0f, 0.0,   1.0f,0.0f


	};
	unsigned int Wall_ind[] = {
		0, 1, 3,
		1,2,3,
		2,5,3,
		5,4,3


	};

	// Våra olika bufferar. Dessa gör så vi kan skicka stora delar vertiser samtidigt så vi slipper skicka 1 i taget
	//VBO(vertex buffer object) skickar våra vertriser till GPU'n
	//Alla buffrar måste vara unsigned ints
	unsigned int VBO[2], VAO[2], EBO[2];
	glGenVertexArrays(2, VAO);
	
	//Skapar ett eller flera buffer objekt
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	glBindVertexArray(VAO[0]);

	//Binder Buffern till det sóm den skall bindas till
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(prism), prism, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prism_ind), prism_ind, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wall), Wall, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Wall_ind), Wall_ind, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Gör så vi har en depth buffer, dvs en z buffer så opengl vet vad som ligger bakom och framför 
	glEnable(GL_DEPTH_TEST);

	
	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("Texture/checker.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//Modell och view matris
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	
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

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);
		// render container
		ourShader.use();

		//transform
		model = glm::rotate(model, deltaTime * glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniform1f(glGetUniformLocation(ourShader.ID, "type_in"), 0.0f);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(VAO[1]);
		glUniform1f(glGetUniformLocation(ourShader.ID, "type_in"), 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));


		glDrawElements(GL_TRIANGLES, 4* 3, GL_UNSIGNED_INT, 0);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);

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
	float cameraSpeed = 2.5f * deltaTime;
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
