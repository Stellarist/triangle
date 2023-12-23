#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

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

class Mesh{
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(Shader& shader);

friend class Model;
};
