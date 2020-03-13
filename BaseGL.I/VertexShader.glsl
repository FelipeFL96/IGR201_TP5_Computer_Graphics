#version 450 core // Minimal GL version support expected from the GPU

layout(location=0) in vec3 vPosition; // The 1st input attribute is the position (CPU side: glVertexAttrib 0)
layout(location=1) in vec3 vColor; // The 2nd input attribute is the vertex color (CPU side: glVertexAttrib 1)

uniform mat4 projectionMat, modelViewMat;

out vec3 fColor; // The vertex shader outpus a vec3 capturing vertex color

void main() {
    gl_Position =  projectionMat * modelViewMat * vec4 (vPosition, 1.0);; // mandatory to fire rasterization properly
    fColor = vec3  (vColor); // Output passed to the next stage, interpolated at fragment barycentric coord. by default
}
