#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=3) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
// TODO(student): Output values to fragment shader
out vec3 f_color;
out vec2 xz;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898, 78.233))) 
                 * 11241.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation
    vec2 u = f * f * (3.0 - 2.0 * f);  // Cubic Hermite curve
    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

void main()
{
    // TODO(student): Send output to fragment shader
    f_color = color;
    xz = position.xz;

    vec3 aux = position;

    aux.y = noise(position.xz) * 0.5;
    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(aux, 1);
}
