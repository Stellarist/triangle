#include <print>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/Camera.hpp"
#include "utils/Shader.hpp"
#include "utils/Texture.hpp"
#include "utils/VertexArray.hpp"
#include "utils/VertexBuffer.hpp"
#include "utils/VertexLayout.hpp"

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
    // load glfw
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::println("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    Shader our_shader(PROJECT_PATH"/shaders/triangle.vs", PROJECT_PATH"/shaders/triangle.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cube_positions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // create and bind buffers
    VertexArray vao;
    VertexBuffer vbo(vertices, 5*6*6*sizeof(float));
    VertexLayout vlo;
    vlo.push<float>(3);
    vlo.push<float>(2);
    vao.addBuffer(vbo, vlo);

    // creating textures
    Texture::flipVertically(true);
    Texture texture1(PROJECT_PATH"/assets/textures/container.jpg");
    Texture texture2(PROJECT_PATH"/assets/textures/awesomeface.png");

    our_shader.use();
    our_shader.setInt("texture1", 0);
    our_shader.setInt("texture2", 1);

    //rendering
    while (!glfwWindowShouldClose(window)) {
        float current_flame_time=(float)glfwGetTime();
        delta_time=current_flame_time-last_flame_time;
        last_flame_time=current_flame_time;

        processInput(window);

        // clear buffer color
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        texture1.bind(0);
        texture2.bind(1);

        // rendering
        our_shader.use();

        glm::mat4 view=camera.getView();
        our_shader.setMat4("view", view);

        glm::mat4 projection=camera.getProjection();
        our_shader.setMat4("projection", projection);

        vao.bind();
        for(unsigned int i=0; i<10; i++){
            glm::mat4 model(1.0f);
            model=glm::translate(model, cube_positions[i]);
            float angle=20.0f*i*glfwGetTime();
            model=glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            our_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }        

        // swap buffers and inquire io event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
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
