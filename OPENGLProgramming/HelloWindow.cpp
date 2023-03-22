#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;


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

	//rendering loop
	float red;

	while (!glfwWindowShouldClose(window)) {

		//INPUT
		processInput(window);

		//RENDERING COMMANDS HERE
		red = (1 + sin(glfwGetTime())) / 2.0;
		glClearColor(red, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//CHECK AND CALL EVENTS AND SWAP DOUBLE BUFFERS
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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