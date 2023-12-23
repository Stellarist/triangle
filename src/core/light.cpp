#define STB_IMAGE_IMPLEMENTATION

#include <print>
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader box_shader(PROJECT_PATH"/shaders/box.vs", PROJECT_PATH"/shaders/box.fs");
    Shader light_shader(PROJECT_PATH"/shaders/light.vs", PROJECT_PATH"/shaders/light.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

    glm::vec3 point_light_positions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    VertexArray box_vao;
    VertexBuffer box_vbo(vertices, sizeof(vertices));
    VertexLayout box_layout;
    box_layout.push<float>(3);
    box_layout.push<float>(3);
    box_layout.push<float>(2);
    box_vao.addBuffer(box_vbo, box_layout);

    VertexArray light_vao;
    VertexBuffer light_vbo(vertices, sizeof(vertices));
    VertexLayout light_layout;
    light_layout.push<float>(3);
    light_layout.push<float>(3);
    light_layout.push<float>(2);
    light_vao.addBuffer(light_vbo, light_layout);

    unsigned int diffuse_map=loadTexture(PROJECT_PATH"/assets/textures/container2.png");
    unsigned int specular_map=loadTexture(PROJECT_PATH"/assets/textures/container2_specular.png");

    light_shader.use();
    light_shader.setInt("material.diffuse", 0);
    light_shader.setInt("material.specular", 1);

    while(!glfwWindowShouldClose(window)){
        float current_flame_time=(float)glfwGetTime();
        delta_time=current_flame_time-last_flame_time;
        last_flame_time=current_flame_time;

        processInput(window);

        // clear buffer color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time=current_flame_time;
        glm::vec3 light_pos(2.0f*cos(time)*sin(time), 2.0f*sin(time)*sin(time), 2.0f*cos(time));

        box_shader.use();
        box_shader.setVec3("view_pos", camera.getPosition());
        box_shader.setFloat("material.shininess", 32.0f);

        box_shader.setVec3("dir_light.direction", -0.2f, -1.0f, -0.3f);
        box_shader.setVec3("dir_light.ambient", 0.05f, 0.05f, 0.05f);
        box_shader.setVec3("dir_light.diffuse", 0.4f, 0.4f, 0.4f);
        box_shader.setVec3("dir_light.specular", 0.5f, 0.5f, 0.5f);

        box_shader.setVec3("point_light[0].position", point_light_positions[0]);
        box_shader.setVec3("point_light[0].ambient", 0.05f, 0.05f, 0.05f);
        box_shader.setVec3("point_light[0].diffuse", 0.8f, 0.8f, 0.8f);
        box_shader.setVec3("point_light[0].specular", 1.0f, 1.0f, 1.0f);
        box_shader.setFloat("point_light[0].constant", 1.0f);
        box_shader.setFloat("point_light[0].linear", 0.09f);
        box_shader.setFloat("point_light[0].quadratic", 0.032f);

        box_shader.setVec3("point_light[1].position", point_light_positions[1]);
        box_shader.setVec3("point_light[1].ambient", 0.05f, 0.05f, 0.05f);
        box_shader.setVec3("point_light[1].diffuse", 0.8f, 0.8f, 0.8f);
        box_shader.setVec3("point_light[1].specular", 1.0f, 1.0f, 1.0f);
        box_shader.setFloat("point_light[1].constant", 1.0f);
        box_shader.setFloat("point_light[1].linear", 0.09f);
        box_shader.setFloat("point_light[1].quadratic", 0.032f);

        box_shader.setVec3("point_light[2].position", point_light_positions[2]);
        box_shader.setVec3("point_light[2].ambient", 0.05f, 0.05f, 0.05f);
        box_shader.setVec3("point_light[2].diffuse", 0.8f, 0.8f, 0.8f);
        box_shader.setVec3("point_light[2].specular", 1.0f, 1.0f, 1.0f);
        box_shader.setFloat("point_light[2].constant", 1.0f);
        box_shader.setFloat("point_light[2].linear", 0.09f);
        box_shader.setFloat("point_light[2].quadratic", 0.032f);

        box_shader.setVec3("point_light[3].position", point_light_positions[3]);
        box_shader.setVec3("point_light[3].ambient", 0.05f, 0.05f, 0.05f);
        box_shader.setVec3("point_light[3].diffuse", 0.8f, 0.8f, 0.8f);
        box_shader.setVec3("point_light[3].specular", 1.0f, 1.0f, 1.0f);
        box_shader.setFloat("point_light[3].constant", 1.0f);
        box_shader.setFloat("point_light[3].linear", 0.09f);
        box_shader.setFloat("point_light[3].quadratic", 0.032f);

        box_shader.setVec3("spot_light.position", camera.getPosition());
        box_shader.setVec3("spot_light.direction", camera.getFront());
        box_shader.setVec3("spot_light.ambient", 0.0f, 0.0f, 0.0f);
        box_shader.setVec3("spot_light.diffuse", 1.0f, 1.0f, 1.0f);
        box_shader.setVec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
        box_shader.setFloat("spot_light.constant", 1.0f);
        box_shader.setFloat("spot_light.linear", 0.09f);
        box_shader.setFloat("spot_light.quadratic", 0.032f);
        box_shader.setFloat("spot_light.cutOff", glm::cos(glm::radians(12.5f)));
        box_shader.setFloat("spot_light.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 model(1.0f);
        box_shader.setMat4("model", model);

        glm::mat4 view=camera.getView();
        box_shader.setMat4("view", view);

        glm::mat4 projection=camera.getProjection();
        box_shader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_map);

        box_vao.bind();
        for(size_t i=0; i<10; i++){
            glm::mat4 mode=glm::mat4(1.0f);
            model=glm::translate(mode, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            box_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        light_shader.use();
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("view", view);
    
        light_vao.bind();
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            light_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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
        std::println("Texture failed to load at path: ", path);
        stbi_image_free(data);
    }

    return textureID;
}
