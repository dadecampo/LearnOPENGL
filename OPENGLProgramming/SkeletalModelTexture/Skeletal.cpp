#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "animator.h"
#include "model_animation.h"

#include <string>
#include <windows.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
std::string workingdir()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 700;

// camera
glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraStartAt = glm::vec3(0.0f, 2.0f, 0.0f);
float cameraDistanceToLookAt = 10.0f;
Camera camera(cameraStartAt);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	// build and compile shaders
	// -------------------------
	Shader ourShader("SkeletalModelTexture\\anim_model.vs", "SkeletalModelTexture\\anim_model.fs");
	Shader houseShader("SkeletalModelTexture\\model_loading_vs.glsl", "SkeletalModelTexture\\model_loading_fs.glsl");

	Shader pupilShader("SkeletalModelTexture\\pupil_shader.vs", "SkeletalModelTexture\\pupil_shader.fs");
	Shader lightCubeShader("SkeletalModelTexture\\light_cube_vs.glsl", "SkeletalModelTexture\\light_cube_fs.glsl");

	std::string link = "/LinkPraying/link.fbx";
	std::string gonzo = "/Gonzo/Gonzo.fbx";
	std::string orcaHouse = "/Orca/OrcaHouse.fbx";


	// load models
	// -----------
	Model linkModel(("Assets" + link).c_str(), "", false);
	Animation prayingAnimation(("Assets" + link).c_str(), &linkModel, 32);
	Animator linkAnimator(&prayingAnimation);

	Model gonzoModel(("Assets" + gonzo).c_str(), "", false);
	Animation tauntingAnimation(("Assets" + gonzo).c_str(), &gonzoModel);
	Animator gonzoAnimator(&tauntingAnimation);

	Model orcaHouseModel(("Assets" + orcaHouse).c_str(), "", false);

	float vertices[] = {
		// positions        
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	//pointlight positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  1.0f,  -3.0f),
		glm::vec3(0.0f,  0.0f,  -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  0.0f)
	};

	glm::vec3 dirLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 spotLightColor = glm::vec3(1.0f);

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// first, configure the cube's VAO (and VBO)
	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float rotationAngle = 0.0f;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		linkAnimator.UpdateAnimation(deltaTime);
		gonzoAnimator.UpdateAnimation(deltaTime);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClearDepth(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cameraLookAt = glm::vec3(0.0f, cameraLookAt.y, 0.0f);

		camera.Position = glm::vec3(cameraStartAt.x + cameraDistanceToLookAt * cos((float)glfwGetTime()), cameraStartAt.y + (sin((float)glfwGetTime()) / 2), cameraStartAt.z + cameraDistanceToLookAt * sin((float)glfwGetTime()));
		glm::mat4 view = glm::lookAt(camera.Position, cameraLookAt, camera.Up);
		camera.Yaw = 0;
		camera.Pitch = 0;

		rotationAngle += 1.0f;

		// point light 1
		glm::vec3 pointLightPositionMoved[] = { pointLightPositions[0], pointLightPositions[1] };
		pointLightPositionMoved[0] = glm::vec3(pointLightPositions[0].x + 2 * cos((float)glfwGetTime() * 3), pointLightPositions[0].y + 0.5 * (cos((float)glfwGetTime()) * 10) * (sin((float)glfwGetTime()) / 2), pointLightPositions[0].z + 2.5 * sin((float)glfwGetTime() * 3));
		pointLightPositionMoved[1] = glm::vec3(pointLightPositions[1].x + 2 * cos((float)glfwGetTime() * 5), pointLightPositions[1].y + 0.5 * (cos((float)glfwGetTime()) * 5) * (sin((float)glfwGetTime()) / 2), pointLightPositions[1].z + 2.5 * sin((float)glfwGetTime() * 5));

		// don't forget to enable shader before setting uniforms
		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);

		ourShader.setFloat("material.shininess", 8.0f);
		ourShader.setVec3("dirLight.direction", 1.0f, -1.0f, 1.0f);
		ourShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
		ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specular", 0.15f, 0.15f, 0.15f);
		ourShader.setVec3("dirLight.color", dirLightColor);

		ourShader.setVec3("pointLights[0].position", pointLightPositionMoved[0]);
		ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[0].specular", 0.2f, 0.2f, 0.2f);
		ourShader.setFloat("pointLights[0].constant", 1.0f);
		ourShader.setFloat("pointLights[0].linear", 0.09f);
		ourShader.setFloat("pointLights[0].quadratic", 0.032f);
		ourShader.setVec3("pointLights[0].color", pointLightColors[0]);

		ourShader.setVec3("pointLights[1].position", pointLightPositionMoved[1]);
		ourShader.setVec3("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
		ourShader.setVec3("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("pointLights[1].specular", 0.1f, 0.1f, 0.1f);
		ourShader.setFloat("pointLights[1].constant", 1.0f);
		ourShader.setFloat("pointLights[1].linear", 0.2f);
		ourShader.setFloat("pointLights[1].quadratic", 0.032f);
		ourShader.setVec3("pointLights[1].color", pointLightColors[1]);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = glm::lookAt(camera.Position, cameraLookAt, camera.Up);

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		auto transformsLink = linkAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transformsLink.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsLink[i]);


		// render the loaded model
		glm::mat4 modelLink = glm::mat4(1.0f);
		modelLink = glm::translate(modelLink, glm::vec3(0.0f, -2.0f, -3.0f)); // translate it down so it's at the center of the scene
		modelLink = glm::scale(modelLink, glm::vec3(.00025f, .00025f, .00025f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", modelLink);
		linkModel.Draw(ourShader);


		auto transforms = gonzoAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 3.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene

		model = glm::scale(model, glm::vec3(.00025f, .00025f, .00025f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		gonzoModel.Draw(ourShader);


		houseShader.use();
		houseShader.setVec3("viewPos", camera.Position);
		houseShader.setFloat("material.shininess", 1.0f);

		houseShader.setVec3("dirLight.direction", 1.0f, -1.0f, 1.0f);
		houseShader.setVec3("dirLight.ambient", 0.08f, 0.08f, 0.08f);
		houseShader.setVec3("dirLight.diffuse", 0.15f, 0.15f, 0.15f);
		houseShader.setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);
		houseShader.setVec3("dirLight.color", dirLightColor);

		houseShader.setVec3("pointLights[0].position", pointLightPositionMoved[0]);
		houseShader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
		houseShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		houseShader.setVec3("pointLights[0].specular", 0.2f, 0.2f, 0.2f);
		houseShader.setFloat("pointLights[0].constant", 0.0f);
		houseShader.setFloat("pointLights[0].linear", 0.09f);
		houseShader.setFloat("pointLights[0].quadratic", 0.032f);
		houseShader.setVec3("pointLights[0].color", pointLightColors[0]);
		
		houseShader.setVec3("pointLights[1].position", pointLightPositionMoved[1]);
		houseShader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
		houseShader.setVec3("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
		houseShader.setVec3("pointLights[1].specular", 0.1f, 0.1f, 0.1f);
		houseShader.setFloat("pointLights[1].constant", 0.0f);
		houseShader.setFloat("pointLights[1].linear", 0.2f);
		houseShader.setFloat("pointLights[1].quadratic", 0.032f);
		houseShader.setVec3("pointLights[1].color", pointLightColors[1]);

		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = glm::lookAt(camera.Position, cameraLookAt, camera.Up);

		houseShader.setMat4("projection", projection);
		houseShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(.018f, .018f, .018f));	// it's a bit too big for our scene, so scale it down
		houseShader.setMat4("model", model);
		orcaHouseModel.Draw(houseShader);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositionMoved[i]);
			model = glm::scale(model, glm::vec3(0.08f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			lightCubeShader.setVec3("color", pointLightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		pupilShader.use();

		pupilShader.setMat4("projection", projection);
		pupilShader.setMat4("model", modelLink);
		pupilShader.setMat4("view", view);
		for (int i = 0; i < transformsLink.size(); ++i)
			pupilShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsLink[i]);

		linkModel.DrawFace(pupilShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	return;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraLookAt.y += yoffset*0.2;
}