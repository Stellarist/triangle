#version 450 core

layout (location=0) in vec3 a_pos;
layout (location=1) in vec2 a_texture_coord;

out vec2 texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position=projection*view*model*vec4(a_pos, 1.0f);
    texture_coord=vec2(a_texture_coord.x, a_texture_coord.y);
}
