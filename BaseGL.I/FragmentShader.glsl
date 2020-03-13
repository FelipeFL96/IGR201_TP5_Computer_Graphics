#version 450 core // Minimal GL version support expected from the GPU

in vec3 fColor; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)

out vec4 color; // Shader output: the color response attached to this fragment

void main() {
	color = vec4 (fColor, 1.0);
}