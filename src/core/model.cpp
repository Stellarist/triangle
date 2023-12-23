#define STB_IMAGE_IMPLEMENTATION

#include <print>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/Camera.hpp"
#include "utils/Model.hpp"

int SCR_WIDTH=1920;
int SCR_HEIGHT=1080;

void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow* window, double x_pos_in, double y_pos_in);
void scrollCallback(GLFWwindow* window, double x_ofs, double y_ofs);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x=SCR_WIDTH/2.0f;
float last_y=SCR_HEIGHT/2.0f;
bool first_mouse=true;

float delta_time=0.0f;
float last_flame_time=0.0f;

int main(int argc, char const* argv[])
{
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle", NULL, NULL);
    if (window == NULL) {
        std::println("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        return -1;
    }

    Texture::flipVertically();
    Shader our_shader(PROJECT_PATH"/shaders/model.vs", PROJECT_PATH"/shaders/model.fs");
    Model our_model(PROJECT_PATH"/assets/models/Nanosuit/Nanosuit.obj");

    while(!glfwWindowShouldClose(window)){
        float current_flame_time=(float)glfwGetTime();
        delta_time=current_flame_time-last_flame_time;
        last_flame_time=current_flame_time;

        processInput(window);

        // clear buffer color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        our_shader.use();

        // origin object
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        glm::mat4 model(1.0f);
        model=glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model=glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        our_shader.setMat4("model", model);

        glm::mat4 view=camera.getView();
        our_shader.setMat4("view", view);

        glm::mat4 projection=camera.getProjection();
        our_shader.setMat4("projection", projection);

        bool border=false;
        our_shader.setBool("border", border);

        our_model.draw(our_shader);

        // // border outline
        // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilMask(0x00);
        // glDisable(GL_DEPTH_TEST);

        // our_shader.setMat4("model", model);

        // border=true;
        // our_shader.setBool("border", border);

        // our_model.draw(our_shader);

        // disable stencil test
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF); 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, delta_time);
    if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, delta_time);
    if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, delta_time);
    if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, delta_time);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    float x_pos=(float)x_pos_in;
    float y_pos=(float)y_pos_in;

    if(first_mouse){
        last_x=x_pos;
        last_y=y_pos;
        first_mouse=false;
    }

    float x_ofs=x_pos-last_x;
    float y_ofs=-(y_pos-last_y);

    last_x=x_pos;
    last_y=y_pos;

    camera.processMouseMovement(x_ofs, y_ofs);
}

void scrollCallback(GLFWwindow* window, double x_ofs, double y_ofs)
{
    camera.processMouseScroll((float)y_ofs);
}
