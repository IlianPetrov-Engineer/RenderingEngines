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
//#include "Light.h"

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

std::string readFileToString(const std::string& filePath) {
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
}

float finishFrameTime = 0.0f;
float deltaTime = 0.0f;

bool activeMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = g_width / 2.0;
float lastY = g_height / 2.0;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

    glDeleteShader(modelVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({ quad });
    quadModel.translate(glm::vec3(0, 0, -2.5));
    quadModel.scale(glm::vec3(5, 5, 1));

    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
        clearColor.g, clearColor.b, clearColor.a);

    Camera cam;

    //VP
    glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
    GLint textureModelUniform = glGetUniformLocation(textureShaderProgram, "mvpMatrix");
    GLint textureUniform = glGetUniformLocation(textureShaderProgram, "text");
    GLint lightDirectionUniform = glGetUniformLocation(modelShaderProgram, "lightDirection");

    float rotationStrength = 100.0f;

    float zoom = 0;

    float camSpeed = 2.5f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Raw Engine v2 - BC4");
        ImGui::Text("Hello :)");
        ImGui::SliderFloat("Camera speed: ", &camSpeed, 0, 200);
        ImGui::SliderFloat("Camera rotation speed: ", &rotationStrength, -1000, 1000);
        ImGui::End();

        processInput(window);
        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));


        float currentTime = glfwGetTime();
        deltaTime = currentTime - finishFrameTime;
        finishFrameTime = currentTime;

        float speed = static_cast<float>(camSpeed * deltaTime);

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

        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

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
 
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            zoom += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            zoom -= 0.01f;
       
        projection = glm::perspective(glm::radians(45.0f + zoom), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

        view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.up);
           
        printf("Camera pos: (%f, %f, %f)\n", cam.cameraPos.x, cam.cameraPos.y, cam.cameraPos.z);

        glUseProgram(textureShaderProgram);
        glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * quadModel.getModelMatrix()));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        quadModel.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        /*float timeValue = glfwGetTime();
        float greenValue = (cos(timeValue) / 2.0f) + 0.5f;

        glClearColor(clearColor.r,
            greenValue, clearColor.b, clearColor.a);*/

        glUseProgram(modelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne.getModelMatrix()));
        glUniform3f(lightDirectionUniform, 1, 0, 0);
        suzanne.render();
        glBindVertexArray(0);

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
