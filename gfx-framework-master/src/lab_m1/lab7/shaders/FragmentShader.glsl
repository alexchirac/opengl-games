#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
in vec2 res;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec2 u_resolution;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color, 0);
    u_resolution = res;

}
