#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"

#include "Camera.h"
#include "Light.h"
#include "SceneManager.h"
//#include "AllScenes.h"
#include "PostProcessing.h"
#include "DefaultPrograms.h"

//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 800;
int g_height = 600;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window,
	int width, int height) {
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

/*std::string readFileToString(const std::string& filePath) {
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		printf("Could not open file: %s\n", filePath.c_str());
		return "";
	}
	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}

GLuint generateShader(const std::string& shaderPath, GLuint shaderType) {
	printf("Loading shader: %s\n", shaderPath.c_str());
	const std::string shaderText = readFileToString(shaderPath);
	const GLuint shader = glCreateShader(shaderType);
	const char* s_str = shaderText.c_str();
	glShaderSource(shader, 1, &s_str, nullptr);
	glCompileShader(shader);
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
	}
	return shader;
}*/

#pragma region Variables

float finishFrameTime = 0.0f;
float deltaTime = 0.0f;

float rotationStrength = 100.0f;

int renderSetting = 0;

#pragma region Mouse input

bool activeMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = g_width / 2.0;
float lastY = g_height / 2.0;

float zoom = 0;
float camSpeed = 50.0f;

#pragma endregion

#pragma region Light

glm::vec4 ambientColour = glm::vec4(1, 0.01, 0.01, 1);
glm::vec4 objectColor = glm::vec4(1, 1, 0.01, 1);
glm::vec4 lightColor = glm::vec4(1, 1, 1, 1);

glm::vec3 lightPos = glm::vec3(1, 0, 0);

float ambientStrength = 0.5f;
float specularStrength = 32.0f;

float diffuseStength = 1.0f;

#pragma endregion

#pragma endregion


int main() {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	g_width = mode->width;
	g_height = mode->height * 0.925;

	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//Setup platforms
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const GLuint modelVertexShader = generateShader("shaders/modelVertex.vs", GL_VERTEX_SHADER);
	const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);
	const GLuint textureShader = generateShader("shaders/texture.fs", GL_FRAGMENT_SHADER);
	const GLuint lightFragmentShader = generateShader("Light.fs", GL_FRAGMENT_SHADER);
	const GLuint lightVertexShader = generateShader("Light.vs", GL_VERTEX_SHADER);

#pragma region Programs

	int success;
	char infoLog[512];
	const unsigned int modelShaderProgram = glCreateProgram(); // shader used for the monkey uses modelVertex as vertex shader, and fragment.fs as fragment shader
	glAttachShader(modelShaderProgram, modelVertexShader);
	glAttachShader(modelShaderProgram, fragmentShader);
	glLinkProgram(modelShaderProgram);
	glGetProgramiv(modelShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(modelShaderProgram, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}

	const unsigned int textureShaderProgram = glCreateProgram();
	glAttachShader(textureShaderProgram, modelVertexShader);
	glAttachShader(textureShaderProgram, textureShader);
	glLinkProgram(textureShaderProgram);
	glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(textureShaderProgram, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}

	const unsigned int lightShaderProgram = glCreateProgram();
	glAttachShader(lightShaderProgram, lightVertexShader);
	glAttachShader(lightShaderProgram, lightFragmentShader);
	glLinkProgram(lightShaderProgram);
	glGetProgramiv(lightShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(lightShaderProgram, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}

	glDeleteShader(modelVertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(textureShader);
	glDeleteShader(lightFragmentShader);
	glDeleteShader(lightVertexShader);

#pragma endregion

#pragma region Uniforms

	GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
	GLint textureModelUniform = glGetUniformLocation(textureShaderProgram, "mvpMatrix");
	GLint textureUniform = glGetUniformLocation(textureShaderProgram, "text");

	GLint modelMatrixLightUniformLocation = glGetUniformLocation(lightShaderProgram, "model");
	GLint viewMatrixLightUniformLocation = glGetUniformLocation(lightShaderProgram, "view");
	GLint projectionMatrixLightUniformLocation = glGetUniformLocation(lightShaderProgram, "projection");

	GLint ambientColourUniformLocation = glGetUniformLocation(lightShaderProgram, "ambientColour");
	GLint objectColorUniformLocation = glGetUniformLocation(lightShaderProgram, "objectColor");
	GLint lightColorUniformLocation = glGetUniformLocation(lightShaderProgram, "lightColor");

	GLint lightPosUniformLocation = glGetUniformLocation(lightShaderProgram, "lightPos");
	GLint viewPosUniformLocation = glGetUniformLocation(lightShaderProgram, "viewPos");

	GLint ambientStrengthUniformLocation = glGetUniformLocation(lightShaderProgram, "ambientStrength");
	GLint specularStrengthUniformLocation = glGetUniformLocation(lightShaderProgram, "specularStrength");
	GLint diffuseStrengthUniformLocation = glGetUniformLocation(lightShaderProgram, "diffuseStength");

	//GLint lightDirectionUniform = glGetUniformLocation(modelShaderProgram, "lightDirection");

#pragma endregion

	Scene scene0;
	Scene scene1;
	Scene scene2;

	core::Mesh quad = core::Mesh::generateQuad();
	core::Model quadModel({ quad });
	quadModel.translate(glm::vec3(0, 0, -2.5));
	quadModel.scale(glm::vec3(5, 5, 1));
	
	core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
	/*core::Model apple = core::AssimpLoader::loadModel("models/Apple.obj");
	core::Texture appleTexture("textures/texture.png");*/
	core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");
	core::Texture suzanneTexture("textures/Vol_42_1_Base_Color.png");

	scene0.objects.push_back(GameObject(quadModel, &cmgtGatoTexture, textureShaderProgram, textureModelUniform, textureUniform));
	scene0.objects.push_back(GameObject(suzanne, &suzanneTexture, textureShaderProgram, textureModelUniform, textureUniform));
	scene1.objects.push_back(GameObject(suzanne, NULL, modelShaderProgram, NULL, NULL));
	//scene2.objects.push_back(GameObject(suzanne, NULL, lightShaderProgram, modelMatrixLightUniformLocation, projectionMatrixLightUniformLocation));
	//scene2.objects.push_back(GameObject(apple, &appleTexture, textureShaderProgram, textureModelUniform, textureUniform));

	SceneManager sceneManager;
	sceneManager.scenes.push_back(scene0);
	sceneManager.scenes.push_back(scene1);
	sceneManager.scenes.push_back(scene2);
	 
	//apple.translate(glm::vec3(1,5,1));

	glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glClearColor(clearColor.r,
		clearColor.g, clearColor.b, clearColor.a);

	Camera cam;

	glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

	PostProcessing post;
	post.Initialise(g_width , g_height);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		post.Begin();
		//printf("Wight & height: (%i, %i)\n", g_width, g_height);
		Scene& currentSceene = sceneManager.GetCurrentScene();

#pragma region GUI

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::SetNextWindowSize(ImVec2(g_width * 0.25, g_height), ImGuiCond_Always);
		ImGui::NewFrame();
		ImGui::Begin("Controls");

		if (ImGui::CollapsingHeader("Camera & Object"))
		{
			ImGui::InputFloat("Camera speed", &camSpeed, 0, 0, "%.2f");
			ImGui::InputFloat("Model rotation speed", &rotationStrength, 1, 1, "%.2f");
			ImGui::SliderFloat("Mouse sensitivity", &cam.mouseSensitivity, 0.001f, 1.0f);

			if (ImGui::Button("Reset camera"))
				camSpeed = 50.0f;

			ImGui::SameLine();
			if (ImGui::Button("Reset rotation"))
				rotationStrength = 100.0f;

			ImGui::SameLine();
			if (ImGui::Button("Reset sensitivity"))
				cam.mouseSensitivity = 0.1f;
		}

		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::InputFloat("Ambient light Strength", &ambientStrength, 0, 0, "%.2f", 1);
			if (ambientStrength < -99)
				ambientStrength = -99;
			if (ambientStrength > 9999)
				ambientStrength = 9999;

			ImGui::InputFloat("Specular light Strength", &specularStrength, 0, 0, "%.2f", 1);
			ImGui::InputFloat("Diffuse light Strength", &diffuseStength, 0, 0, "%.2f", 1);

			if (ImGui::Button("Reset ambient light"))
				ambientStrength = 0.5f;
			ImGui::SameLine();
			if (ImGui::Button("Reset Specular light"))
				specularStrength = 32.0f;

			ImGui::ColorPicker4("Ambient colour", &ambientColour.x, ambientColour.y, &ambientColour.z);
			ImGui::ColorPicker4("Object colour", &objectColor.x, objectColor.y, &objectColor.z);
			ImGui::ColorPicker4("Light colour", &lightColor.x, lightColor.y, &lightColor.z);
		}

		if (ImGui::CollapsingHeader("Render"))
		{
			/*if (ImGui::Button("Change render program"))
				renderSetting += 1;
			if (renderSetting > 2)
				renderSetting = 0;*/

			if (ImGui::Checkbox("Grayscale", &post.grayscale)) {}
			if (ImGui::Checkbox("Invert", &post.invert)) {}
		}

		if (ImGui::Button("Scene 0"))
			sceneManager.SetScene(0);
		if (ImGui::Button("Scene 1"))
			sceneManager.SetScene(1);
		if (ImGui::Button("Scene 2"))
			sceneManager.SetScene(2);

		/*for (int i = 0; i < currentSceene.objects.size(); i++) 
		{
			if (ImGui::Button("Test: " + currentSceene.objects[i]))
				currentSceene.objects[i].model.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));
		}*/

		ImGui::End();

#pragma endregion

		processInput(window);
		//suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));

		sceneManager.scenes[0].objects[1].model.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));


		float currentTime = glfwGetTime();
		deltaTime = currentTime - finishFrameTime;
		finishFrameTime = currentTime;

		float speed = static_cast<float>(camSpeed * deltaTime);

#pragma region Camera Movement

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.Forward(-speed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.Forward(speed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.Right(-speed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.Right(speed);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cam.Up(speed);
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			cam.Up(-speed);

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			zoom += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			zoom -= 0.01f;

#pragma endregion

#pragma region Camera rotation

		bool cursorLock = false;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorLock = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorLock = false;
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (cursorLock)
		{
			if (activeMouse)
			{
				lastX = xpos;
				lastY = ypos;
				activeMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;

			cam.Rotate(xoffset, yoffset);
		}

#pragma endregion

		projection = glm::perspective(glm::radians(45.0f + zoom), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

		view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.up);

		glUseProgram(textureShaderProgram);
		/*glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * quadModel.getModelMatrix()));
		glActiveTexture(GL_TEXTURE2);*/
		glUniform1i(textureUniform, 2);
		/*glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
		quadModel.render();
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);*/


		if (renderSetting == 0)
		{
			glUseProgram(modelShaderProgram);
			/*glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne.getModelMatrix()));
			glUniform3f(lightDirectionUniform, 1, 0, 0
			glBindVertexArray(0);*/
		}

		if (renderSetting == 1)
		{
			glUseProgram(textureShaderProgram);
			/*glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne.getModelMatrix()));
			glActiveTexture(GL_TEXTURE0);*/
			glUniform1i(textureUniform, 0);
			/*glBindTexture(GL_TEXTURE_2D, suzanneTexture.getId());
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);*/
		}
		
		else if (renderSetting == 2)
		{
			glUseProgram(lightShaderProgram);
			glUniformMatrix4fv(modelMatrixLightUniformLocation, 1, GL_FALSE, glm::value_ptr(suzanne.getModelMatrix()));
			glUniformMatrix4fv(viewMatrixLightUniformLocation, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionMatrixLightUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

			glUniform3f(ambientColourUniformLocation, ambientColour.x, ambientColour.y, ambientColour.z);
			glUniform3f(objectColorUniformLocation, objectColor.x, objectColor.y, objectColor.z);
			glUniform3f(lightColorUniformLocation, lightColor.x, lightColor.y, lightColor.z);

			glUniform3f(lightPosUniformLocation, lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(viewPosUniformLocation, cam.cameraPos.x, cam.cameraPos.y, cam.cameraPos.z);

			glUniform1f(ambientStrengthUniformLocation, ambientStrength);
			glUniform1f(specularStrengthUniformLocation, specularStrength); // light property or material property...?
			glUniform1f(diffuseStrengthUniformLocation, diffuseStength);
		}

		sceneManager.GetCurrentScene().Render(view, projection);

		/*suzanne.render();
		apple.render();*/

		post.End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		finishFrameTime = glfwGetTime();
		deltaTime = static_cast<float>(finishFrameTime - currentTime);
		currentTime = finishFrameTime;
	}

	glDeleteProgram(modelShaderProgram);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
