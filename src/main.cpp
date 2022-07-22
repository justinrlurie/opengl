#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <chrono>


#include "file_io.h"
#include "SOIL/SOIL.h"

int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1600, 900, "OpenGL", nullptr, nullptr); // Windowed
	//GLFWwindow* window =
		//glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // VSync = 1
	std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	GLuint vao; // Stores all of the links between attributes and vbos
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 vertex buffer
	GLfloat vertices[] = {
	//  Position      Color             Texcoords
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};
	std::printf("%u\n", vbo); // Prints 1
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo); // Generates 1 element buffer
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};
	std::printf("%u\n", ebo); // Prints 2
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	
// VERTEX shader **************
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Declare shader
	const std::string VertexSource = io_Write_to_string("./res/shaders/vertex.shader"); // Write shader to std string
	const GLchar* vertexShaderSource = VertexSource.c_str(); // Make pointer to pass to glShaderSource
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Place source into buffer to compile
	// Compile shader
	glCompileShader(vertexShader);
	// Debug VERTEX build
	GLint vertexShaderStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
	char vertexShaderDebugBuffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderDebugBuffer);
// VERTEX shader **************

// FRAGMENT shader ************
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Declare shader
	const std::string FragmentSource = io_Write_to_string("./res/shaders/fragment.shader"); // Write shader to std string
	const GLchar* fragmentShaderSource = FragmentSource.c_str(); // Make pointer to pass to glShaderSource
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Place source into buffer to compile
	// Compile shader
	glCompileShader(fragmentShader); //Compile shader
	// Debug FRAGMENT build
	GLint fragmentShaderStatus;	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);
	char fragmentShaderDebugBuffer[512];
	glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderDebugBuffer);
// FRAGMENT shader ************

	// Declare programs and attach shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor"); //Specifies the buffer that the fragment shader writes to

	// Link and use program
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Textures
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	int width, height;
	unsigned char* img = SOIL_load_image("./res/textures/dna_c.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_free_image_data(img);
	if (img == NULL) {
		std::printf("Error in loading the image\n");
		exit(1);
	}
	else {
		std::printf("Loaded image with a width of %dpx, a height of %dpx\n", width, height);
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	
	//glBindTexture(GL_TEXTURE_2D, 0);

	

	// Declare and write uniforms
	//GLint u_Color = glGetUniformLocation(shaderProgram, "u_Color");
	//glUniform3f(u_Color, 1.0f, 0.0f, 0.0f);

	//Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));

	auto t_start = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window)) //So-called "closed" event loop
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE); //Press "esc" to close window

		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		//glUniform3f(u_Color, (sin(time) + 1.0f) / 2.0f, (sin(time * 2.3f) + 1.0f) / 2.0f, (sin(time * .1f) + 1.0f) / 2.0f);

		//glDrawArrays(GL_TRIANGLES, 0, 3); //Draws the bound vertex array
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); //Swap front and back buffers after finished drawing
	}

	//std::cout << glGetError();

	glfwTerminate();

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return 0;
}