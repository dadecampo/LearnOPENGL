#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

float vertices[] = {
	-0.5f,  -0.5f,	0.0f,
	 0.5f,	-0.5f,	0.0f,
	 0.0f,	 0.5f,	0.0f
};

int main() {

	//inizializzazione glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creazione finestra
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOPENGL", NULL, NULL);

	//check se la creazione della finestra è andata a buon fine
	if (window == NULL) {
		std::cout << "end" << std::endl;
		glfwTerminate();
		return -1;
	}

	//inizializzo il contesto, la finestra che ho appena creato
	glfwMakeContextCurrent(window);

	//inizializzo le callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//chiedo a GLAD di gestire l'interfaccia con la scheda grafica per quanto riguarda opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;"
		"void main(){\n"
		"	gl_Position= vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		//		"	vertexColor= vec4(0.5, 0.5, 0.0, 1.0);\n"
		"	vertexColor = gl_Position;\n"
		"}\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 vertexColor;"
		"void main(){\n"
		//		"	FragColor = vec4(0.5f,0.5f,0.0f,1.0f);\n"
		"	FragColor = vertexColor;\n"
		"}\0";

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	unsigned int VBO;



	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//rendering loop
	while (!glfwWindowShouldClose(window)) {

		//INPUT
		processInput(window);


		//RENDERING COMMANDS HERE
		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//CHECK AND CALL EVENTS AND SWAP DOUBLE BUFFERS
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;


}

//gestione input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

//callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}