#version 330

// Input
in vec3 f_color;
in vec2 xz;

// Output
layout(location = 0) out vec4 out_color;

// 2D Random
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898, 78.233))) 
                 * 11254.5453123);
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

void main() {
    vec2 st = xz * 0.5;

    // Use the noise function to get a value
    float n = noise(st);
    float lower = 0.3;
    float upper = 0.9;

    n = smoothstep(lower, upper, n);

    vec3 colorWithNoise = mix(f_color, vec3(0.114, 0.4, 0.137), n);

    // Output the final color
    out_color = vec4(colorWithNoise, 1.0);
}