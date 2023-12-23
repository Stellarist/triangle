#pragma once

#include <array>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"

const int MAX_BONES_INFLUENCE = 4;

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;

    glm::vec3 tangent;
    glm::vec3 bitangent;

    std::array<int, MAX_BONES_INFLUENCE> bone_ids;
    std::array<float, MAX_BONES_INFLUENCE> bone_weights;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh{
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;


public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(Shader& shader);
    void setupMesh();
};
