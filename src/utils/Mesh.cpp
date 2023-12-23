#include "Mesh.hpp"

#include <glad/glad.h>

#include "VertexLayout.hpp"

#include "debug/debug.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
: vertices(vertices), indices(indices), textures(textures), 
  vao(),
  vbo(vertices.data(), vertices.size()*sizeof(Vertex)), 
  ibo(indices.data(), indices.size())
{
    VertexLayout layout;
    layout.push<float>(3, sizeof(Vertex::position));
    layout.push<float>(3, sizeof(Vertex::normal));
    layout.push<float>(2, sizeof(Vertex::tex_coords));
    layout.push<float>(3, sizeof(Vertex::tangent));
    layout.push<float>(3, sizeof(Vertex::bitangent));
    layout.push<int>(MAX_BONES_INFLUENCE, sizeof(Vertex::bone_ids));
    layout.push<float>(MAX_BONES_INFLUENCE, sizeof(Vertex::bone_weights));
    vao.addBuffer(vbo, layout);
    vao.unbind();
}

void Mesh::draw(Shader& shader)
{
    unsigned int diffuse_n=1;
    unsigned int specular_n=1;
    unsigned int normal_n=1;
    unsigned int height_n=1;

    for(size_t i=0; i<textures.size(); i++){
        glActiveTexture(GL_TEXTURE0+i);
        std::string number;
        std::string name=textures[i].getType();
        if(name=="texture_diffuse")
            number=std::to_string(diffuse_n++);
        else if(name=="texture_specular")
            number=std::to_string(specular_n++);
        else if(name=="texture_normal")
            number=std::to_string(normal_n++);
        else if(name=="texture_height")
            number=std::to_string(height_n++);

        shader.setInt(name+number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].getId());
    }

    vao.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);)
    vao.unbind();
}
