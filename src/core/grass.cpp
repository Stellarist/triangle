#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <print>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "utils/Camera.hpp"
#include "utils/Shader.hpp"
#include "utils/VertexArray.hpp"
#include "utils/VertexBuffer.hpp"
#include "utils/VertexLayout.hpp"

int SCR_WIDTH=1920;
int SCR_HEIGHT=1080;

void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow* window, double x_pos_in, double y_pos_in);
void scrollCallback(GLFWwindow* window, double x_ofs, double y_ofs);
unsigned int loadTexture(const char* path);

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    Shader our_shader(PROJECT_PATH"/shaders/grass.vs", PROJECT_PATH"/shaders/grass.fs");

    float cube_vertices[] = {
        // positions          // texture Coords
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

    float plane_vertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    float transparent_vertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    // cube VAO
    VertexArray cube_vao;
    VertexBuffer cube_vbo(cube_vertices, sizeof(cube_vertices));
    VertexLayout cube_layout;
    cube_layout.push<float>(3);
    cube_layout.push<float>(2);
    cube_vao.addBuffer(cube_vbo, cube_layout);

    // plane VAO
    VertexArray plane_vao;
    VertexBuffer plane_vbo(plane_vertices, sizeof(plane_vertices));
    VertexLayout plane_layout;
    plane_layout.push<float>(3);
    plane_layout.push<float>(2);
    plane_vao.addBuffer(plane_vbo, plane_layout);

    // transparent VAO
    VertexArray transparent_vao;
    VertexBuffer transparent_vbo(transparent_vertices, sizeof(transparent_vertices));
    VertexLayout transparent_layout;
    transparent_layout.push<float>(3);
    transparent_layout.push<float>(2);
    transparent_vao.addBuffer(transparent_vbo, transparent_layout);

    unsigned int cube_texture=loadTexture(PROJECT_PATH"/assets/textures/marble.jpg");
    unsigned int floor_texture=loadTexture(PROJECT_PATH"/assets/textures/metal.png");
    unsigned int transparent_texture=loadTexture(PROJECT_PATH"/assets/textures/window.png");

    std::vector<glm::vec3> windows ={
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3 (0.5f, 0.0f, -0.6f)
    };

    our_shader.use();
    our_shader.setInt("texture1", 0);

    while(!glfwWindowShouldClose(window)){
        float current_flame_time=(float)glfwGetTime();
        delta_time=current_flame_time-last_flame_time;
        last_flame_time=current_flame_time;

        processInput(window);

        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++){
            float distance = glm::length(camera.getPosition() - windows[i]);
            sorted[distance] = windows[i];
        }

        // clear buffer color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        our_shader.use();
        glm::mat4 model=glm::mat4(1.0f);

        glm::mat4 view=camera.getView();
        our_shader.setMat4("view", view);

        glm::mat4 projection=camera.getProjection();
        our_shader.setMat4("projection", projection);

        // cubes
        cube_vao.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_texture);

        model=glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        our_shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model=glm::mat4(1.0f);
        model=glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        our_shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // floor
        plane_vao.bind();
        glBindTexture(GL_TEXTURE_2D, floor_texture);
        model=glm::mat4(1.0f);
        our_shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // vegetation
        transparent_vao.bind();
        glBindTexture(GL_TEXTURE_2D, transparent_texture);

        for(const auto& [key, value]: sorted){
            model=glm::mat4(1.0f);
            model=glm::translate(model, value);
            our_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

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

unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format=0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::println("Texture failed to load at path:{}", path);
        stbi_image_free(data);
    }

    return textureID;
}
