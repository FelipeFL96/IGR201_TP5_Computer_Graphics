#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Mesh.hpp"

void Mesh::init () {
    initGPUGeometry ();
}

void Mesh::render () {
    glBindVertexArray (vao); // Activate the VAO storing geometry data
    glDrawElements (GL_TRIANGLES, triangleIndices.size (), GL_UNSIGNED_INT, 0); // Call for rendering: stream the current GPU geometry through the current GPU program
}

void Mesh::clear () {
    glDeleteVertexArrays (1, &vao);
    glDeleteBuffers (1, &posVbo);
    glDeleteBuffers (1, &colVbo);
    glDeleteBuffers (1, &ibo);
}

void Mesh::initGPUGeometry () {
    glCreateBuffers (1, &posVbo); // Generate a GPU buffer to store the positions of the vertices
    size_t vertexBufferSize = sizeof (float) * vertexPositions.size (); // Gather the size of the buffer from the CPU-side vector
    glNamedBufferStorage (posVbo, vertexBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT); // Creta a data store on the GPU
    glNamedBufferSubData (posVbo, 0, vertexBufferSize, vertexPositions.data ()); // Fill the data store from a CPU array

    glCreateBuffers (1, &colVbo); // Generate a GPU buffer to store the colors of the vertices
    size_t vertexColorsBufferSize = sizeof (float) * vertexColors.size (); // Gather the size of the buffer from the CPU-side vector
    glNamedBufferStorage (colVbo, vertexColorsBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT); // Creta a data store on the GPU
    glNamedBufferSubData (colVbo, 0, vertexColorsBufferSize, vertexColors.data ()); // Fill the data store from a CPU array

    glCreateBuffers (1, &ibo); // Same for the index buffer, that stores the list of indices of the triangles forming the mesh
    size_t indexBufferSize = sizeof (unsigned int) * triangleIndices.size ();
    glNamedBufferStorage (ibo, indexBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData (ibo, 0, indexBufferSize, triangleIndices.data ());

    glCreateVertexArrays (1, &vao); // Create a single hangle that joins together attributes (vertex positions, normals) and connectivity (triangles indices)
    glBindVertexArray (vao);

    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, posVbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);

    glEnableVertexAttribArray (1);
    glBindBuffer (GL_ARRAY_BUFFER, colVbo);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray (0); // Desactive the VAO just created. Will be activated at rendering time.
}

std::shared_ptr<Mesh> Mesh::genSphere (size_t resolution) {
    std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>();

    int N = resolution;
    float R = 1.0;
    float theta, fi;
    float x, y, z;
    const float pi = 3.14159;

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            theta = i*2*pi/N;
            fi = j*pi/N-pi/2;

            x = R*cos(theta)*cos(fi);
            y = R*sin(theta)*cos(fi);
            z = R*cos(fi);

            sphere->vertexPositions.push_back(x);
            sphere->vertexPositions.push_back(y);
            sphere->vertexPositions.push_back(z);
            sphere->vertexColors.push_back(1.f);
            sphere->vertexColors.push_back(0.f);
            sphere->vertexColors.push_back((j%3 == 1) ? 1.f : 0.f);
        }
    }

    for (int t = 0; t < (N*N-1); t++) {
        sphere->triangleIndices.push_back(t);
        sphere->triangleIndices.push_back(t+1);
        sphere->triangleIndices.push_back(t+N);
        sphere->triangleIndices.push_back(t+1);
        sphere->triangleIndices.push_back(t+1+N);
        sphere->triangleIndices.push_back(t+N);
    }

    return sphere;
}

std::shared_ptr<Mesh> Mesh::genCone (size_t resolution) {
    std::shared_ptr<Mesh> cone = std::make_shared<Mesh>();

    int N = resolution;
    float theta, radius;
    const float pi = 3.14159;

    float x = 0, y = 0, z = 1;
    cone->vertexPositions.push_back(x);
    cone->vertexPositions.push_back(y);
    cone->vertexPositions.push_back(z);
    cone->vertexColors.push_back(1.f);
    cone->vertexColors.push_back(1.f);
    cone->vertexColors.push_back(0.f);

    x = 0, y = 0, z = -1;
    cone->vertexPositions.push_back(x);
    cone->vertexPositions.push_back(y);
    cone->vertexPositions.push_back(z);
    cone->vertexColors.push_back(1.f);
    cone->vertexColors.push_back(1.f);
    cone->vertexColors.push_back(0.f);

    for (int i = 0; i < N; i++) {
        theta = i*2*pi/N;
        x = cos(theta);
        y = sin(theta);
        z = -1;

        cone->vertexPositions.push_back(x);
        cone->vertexPositions.push_back(y);
        cone->vertexPositions.push_back(z);
        cone->vertexColors.push_back(1.f);
        cone->vertexColors.push_back(1.f);
        cone->vertexColors.push_back(0.f);
    }


    for (int t = 2; t < N+1; t++) {
        cone->triangleIndices.push_back(0);
        cone->triangleIndices.push_back(t);
        cone->triangleIndices.push_back(t+1);
    }
    cone->triangleIndices.push_back(0);
    cone->triangleIndices.push_back(N+1);
    cone->triangleIndices.push_back(2);


    for (int t = 2; t < N+1; t++) {
        cone->triangleIndices.push_back(1);
        cone->triangleIndices.push_back(t);
        cone->triangleIndices.push_back(t+1);
    }
    cone->triangleIndices.push_back(1);
    cone->triangleIndices.push_back(N+1);
    cone->triangleIndices.push_back(2);

    return cone;
}

std::shared_ptr<Mesh> Mesh::genCylinder (size_t resolution) {
    std::shared_ptr<Mesh> cylinder = std::make_shared<Mesh>();

    int N = resolution;
    float theta, radius;
    const float pi = 3.14159;

    float x = 0, y = 0, z = 1;
    cylinder->vertexPositions.push_back(x);
    cylinder->vertexPositions.push_back(y);
    cylinder->vertexPositions.push_back(z);
    cylinder->vertexColors.push_back(1.f);
    cylinder->vertexColors.push_back(0.f);
    cylinder->vertexColors.push_back(1.f);

    x = 0, y = 0, z = -1;
    cylinder->vertexPositions.push_back(x);
    cylinder->vertexPositions.push_back(y);
    cylinder->vertexPositions.push_back(z);
    cylinder->vertexColors.push_back(1.f);
    cylinder->vertexColors.push_back(1.f);
    cylinder->vertexColors.push_back(0.f);

    for (int i = 0; i < N; i++) {
        theta = i*2*pi/N;
        x = cos(theta);
        y = sin(theta);
        z = 1;

        cylinder->vertexPositions.push_back(x);
        cylinder->vertexPositions.push_back(y);
        cylinder->vertexPositions.push_back(z);
        cylinder->vertexColors.push_back(1.f);
        cylinder->vertexColors.push_back(0.f);
        cylinder->vertexColors.push_back(0.f);
    }

    for (int i = 0; i < N; i++) {
        theta = i*2*pi/N;
        x = cos(theta);
        y = sin(theta);
        z = -1;

        cylinder->vertexPositions.push_back(x);
        cylinder->vertexPositions.push_back(y);
        cylinder->vertexPositions.push_back(z);
        cylinder->vertexColors.push_back(0.f);
        cylinder->vertexColors.push_back(1.f);
        cylinder->vertexColors.push_back(1.f);
    }


    for (int t = 2; t < N+1; t++) {
        cylinder->triangleIndices.push_back(0);
        cylinder->triangleIndices.push_back(t);
        cylinder->triangleIndices.push_back(t+1);
    }
        cylinder->triangleIndices.push_back(0);
        cylinder->triangleIndices.push_back(N+1);
        cylinder->triangleIndices.push_back(2);

    for (int t = N+2; t < 2*N+1; t++) {
        cylinder->triangleIndices.push_back(1);
        cylinder->triangleIndices.push_back(t);
        cylinder->triangleIndices.push_back(t+1);
    }
    cylinder->triangleIndices.push_back(1);
    cylinder->triangleIndices.push_back(2*N+1);
    cylinder->triangleIndices.push_back(N+2);


    for (int t = 2; t < N+1; t++) {
        cylinder->triangleIndices.push_back(t);
        cylinder->triangleIndices.push_back(t+1);
        cylinder->triangleIndices.push_back(t+N);

        cylinder->triangleIndices.push_back(t+1);
        cylinder->triangleIndices.push_back(t+N+1);
        cylinder->triangleIndices.push_back(t+N);
    }

    cylinder->triangleIndices.push_back(2+N-1);
    cylinder->triangleIndices.push_back(2);
    cylinder->triangleIndices.push_back(2+N-1+N);

    cylinder->triangleIndices.push_back(2);
    cylinder->triangleIndices.push_back(2+N);
    cylinder->triangleIndices.push_back(2+N-1+N);

    return cylinder;
}

std::shared_ptr<Mesh> Mesh::genCube (size_t resolution) {
    std::shared_ptr<Mesh> cube = std::make_shared<Mesh>();

    cube->vertexPositions = {
         1.f,  1.f,  1.f,
        -1.f,  1.f,  1.f,
        -1.f, -1.f,  1.f,
         1.f, -1.f,  1.f,
         1.f,  1.f, -1.f,
        -1.f,  1.f, -1.f,
        -1.f, -1.f, -1.f,
         1.f, -1.f, -1.f
    };

    cube->vertexColors = { 
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 1.f,
        1.f, 0.f, 0.f
    };

    cube->triangleIndices = {
        0, 1, 2,   0, 2, 3,
        0, 5, 4,   0, 1, 5,
        3, 4, 7,   3, 0, 4,
        2, 7, 6,   2, 3, 7,
        4, 5, 7,   5, 6, 7,
        1, 6, 5,   1, 2, 6,
    };

    return cube;
}

std::shared_ptr<Mesh> Mesh::genTorus (size_t resolution) {
    std::shared_ptr<Mesh> torus = std::make_shared<Mesh>();

    int N = resolution;
    float a = 0.2;
    float b = 1.0 - a;
    float theta, fi;
    float x, y, z;
    const float pi = 3.14159;

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            theta = i*2*pi/N;
            fi = j*2*pi/N;

            x = (b + a*cos(fi))*cos(theta);
            y = (b + a*cos(fi))*sin(theta);
            z = a*sin(fi);

            torus->vertexPositions.push_back(x);
            torus->vertexPositions.push_back(y);
            torus->vertexPositions.push_back(z);
            torus->vertexColors.push_back(0.f);
            torus->vertexColors.push_back(1.f);
            torus->vertexColors.push_back(0.f);
        }
    }

    for (int t = 0; t < (N-1)*(N-1); t++) {
        torus->triangleIndices.push_back(t);
        torus->triangleIndices.push_back(t+1);
        torus->triangleIndices.push_back(t+N);
        torus->triangleIndices.push_back(t+1);
        torus->triangleIndices.push_back(t+1+N);
        torus->triangleIndices.push_back(t+N);
    }

    return torus;
}