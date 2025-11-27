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
// mine:
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

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float rotationStrength = 100.0f;

 
    float zoom = 0;

    /*double oldMouseX = 0;
    double deltaMouseX = 0;

    double camHeading = 0;*/

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Raw Engine v2 - BC4");
        ImGui::Text("Hello :)");
       /* ImGui::SliderFloat("Camera offset: ", &camOffset,-2,2);*/
        ImGui::End();

       /* double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        deltaMouseX = xpos - oldMouseX;
        oldMouseX += deltaMouseX;
        printf("Mouse x %f\n", xpos);*/


        processInput(window);
        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));

        float speed = 0.01f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cam.Forward(speed);
        }
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cam.Forward(-speed);
        }
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cam.Right(speed);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cam.Right(-speed);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cam.Up(speed);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            cam.Up(-speed);
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cam.Rotate(glm::vec3(0, 1, 0), -speed);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cam.Rotate(glm::vec3(0, 1, 0), speed);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            cam.Rotate(cam.cameraRight, speed);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            cam.Rotate(cam.cameraRight, -speed);
        /*if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            cam.Rotate(cam.cameraForward, speed);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            cam.Rotate(cam.cameraForward, -speed);*/
 
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            zoom += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            zoom -= 0.01f;
       
        projection = glm::perspective(glm::radians(45.0f + zoom), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);


        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        //{
        //    if (oldMouseX != deltaMouseX)
        //    {
        //        view = glm::translate(glm::rotate(glm::mat4(1), (float)oldMouseX * 0.001f, glm::vec3(0, 1, 0)), -cam.cameraPos);
        //    }
        //    //view = glm::translate(glm::rotate(glm::mat4(1), (float)oldMouseX * 0.001f, glm::vec3(0, 1, 0)), -cam.cameraPos);
        //}
   
        
        // view goes from world to camera. The camera model matrix would go from camera to world (inverse)
        view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.cameraUp);
            //glm::translate(glm::rotate(glm::mat4(1), (float)camHeading, glm::vec3(0, 1, 0)), -cam.cameraPos);
        // you can manipulate matrices like this:
        //glm::mat4 M(1); // identity matrix
        //glm::translate(M, glm::vec3(0, 1, 0)); // translate

        printf("Camera pos: (%f, %f, %f)\n", cam.cameraPos.x, cam.cameraPos.y, cam.cameraPos.z);

        //auto cameraModel = glm::inverse(view); // glm::translate(glm::rotate(glm::mat4(1), (float)oldMouseX * 0.001f, glm::vec3(0, 1, 0)), -cam.cameraPos);
        //glm::vec4 fwd = view * glm::vec4(0, 0, 1, 0);
        //printf("Camera forward: (%f,%f,%f)\n", fwd.x, fwd.y, fwd.z); // See shader programming BC6 :-)
        ////std::cout << cameraModel << std::endl;

        glUseProgram(textureShaderProgram);
        glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * quadModel.getModelMatrix()));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        quadModel.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        glUseProgram(modelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne.getModelMatrix()));
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