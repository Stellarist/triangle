#version 450 core

out vec4 frag_color;

in vec2 tex_coords;

uniform bool border;
uniform sampler2D texture_diffuse1;

void main()
{
    if(!border)
        frag_color=texture(texture_diffuse1, tex_coords);
    else
        // frag_color=vec4(0.5f, 0.5f, 0.5f, 1.0f);
        frag_color=vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
