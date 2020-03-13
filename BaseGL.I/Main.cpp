// ----------------------------------------------
// Computer Graphics
// Base code for practical assignment
// Introduction to interactive 3D graphics
// application development with modern OpenGL.
//
// Copyright (C) 2018 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#define _USE_MATH_DEFINES
#define MSG(x) std::cout<<x<<std::endl;

#include <glad/glad.h>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

#define SOLUTION

using namespace std;

// Window parameters
static GLFWwindow * window = nullptr;

// GPU objects
static GLuint program; // A GPU program contains at least a vertex shader and a fragment shader

// Basic camera model
Camera camera;

// Executed each time the window is resized. Adjust the aspect ratio and the rendering viewport to the current window.
void windowSizeCallback (GLFWwindow* window, int width, int height) {
	camera.setAspectRatio (static_cast<float>(width) / static_cast<float>(height));
	glViewport (0, 0, (GLint)width, (GLint)height); // Dimension of the rendering region in the window
}

// Executed each time a key is entered.
void keyCallback (GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS && key == GLFW_KEY_UP) {
		// Exercise III.c
	} else if (action == GLFW_PRESS && key == GLFW_KEY_DOWN) {
		// Exercise III.c
	} else if (action == GLFW_PRESS && key == GLFW_KEY_F1) {
		GLint mode[2];
		glGetIntegerv (GL_POLYGON_MODE, mode);
		glPolygonMode (GL_FRONT_AND_BACK, mode[1] == GL_FILL ? GL_LINE : GL_FILL);
	} else if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose (window, true); // Closes the application if the escape key is pressed
	}
}

void initGLFW () {
	// Initialize GLFW, the library responsible for window management
	if (!glfwInit ()) {
		std::cerr << "ERROR: Failed to init GLFW" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	// Before creating the window, set some option flags
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);

	// Create the window
	window = glfwCreateWindow (1024, 768, "Base GL - Computer Graphics - Practical Assignment - OpenGL Intro.", nullptr, nullptr);
	if (!window) {
		std::cerr << "ERROR: Failed to open window" << std::endl;
		glfwTerminate ();
		std::exit (EXIT_FAILURE);
	}

	// Load the OpenGL context in the GLFW window using GLAD OpenGL wrangler
	glfwMakeContextCurrent (window);
	glfwSetWindowSizeCallback (window, windowSizeCallback);
	glfwSetKeyCallback (window, keyCallback);
}

void initOpenGL () {
	glfwMakeContextCurrent (window);
	if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "ERROR: Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		std::exit (EXIT_FAILURE);
	}
	glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
	glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
	glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
	glEnable (GL_DEPTH_TEST); // Enable the z-buffer test in the rasterization
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // specify the background color, used any time the framebuffer is cleared
}

// Loads the content of an ASCII file in a standard C++ string
std::string file2String (const std::string & filename) {
	std::ifstream t (filename.c_str ());
	std::stringstream buffer;
	buffer << t.rdbuf ();
	return buffer.str ();
}

// Loads and compile a shader, before attaching it to a program
void loadShader (GLuint program, GLenum type, const std::string & shaderFilename) {
    GLuint shader = glCreateShader (type); // Create the shader, e.g., a vertex shader to be applied to every single vertex of a mesh
    std::string shaderSourceString = file2String (shaderFilename); // Loads the shader source from a file to a C++ string
    if (shaderSourceString.empty())
    {
        cerr << "No content in shader " << shaderFilename << endl;
        glDeleteShader(shader);
        return;
    }
    const GLchar * shaderSource = (const GLchar *)shaderSourceString.c_str (); // Interface the C++ string through a C pointer
    glShaderSource (shader, 1, &shaderSource, NULL); // Load the vertex shader source code
    glCompileShader (shader);  // THe GPU driver compile the shader
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLsizei len;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );
        GLchar* log = new GLchar[len+1];
        glGetShaderInfoLog( shader, len, &len, log );
        std::cerr << "Compilation error in shader " << shaderFilename << " : " << endl << log << std::endl;
        delete [] log;
        glDeleteShader(shader);
        return;
    }
    glAttachShader (program, shader); // Set the vertex shader as the one ot be used with the program/pipeline
    glDeleteShader (shader);
}

void initGPUProgram () {
	program = glCreateProgram (); // Create a GPU program i.e., a graphics pipeline
	loadShader (program, GL_VERTEX_SHADER, "VertexShader.glsl");
	loadShader (program, GL_FRAGMENT_SHADER, "FragmentShader.glsl");
	glLinkProgram (program); // The main GPU program is ready to be handle streams of polygons
}

void initCamera () {
	int width, height;
	glfwGetWindowSize (window, &width, &height);
	camera.setAspectRatio (static_cast<float>(width) / static_cast<float>(height));
}

void init (vector<std::shared_ptr<Mesh>> meshGroup) {
	initGLFW ();
	initOpenGL ();
	initGPUProgram ();
	initCamera ();

	vector<std::shared_ptr<Mesh>>::iterator mesh;
	for(mesh = meshGroup.begin(); mesh != meshGroup.end(); ++mesh) {
		(*mesh)->init();
	}
}

void render (vector<std::shared_ptr<Mesh>> meshGroup) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.
	glUseProgram (program); // Activate the program to be used for upcoming primitive

    glm::mat4 projectionMatrix = camera.computeProjectionMatrix();
    glUniformMatrix4fv (glGetUniformLocation (program, "projectionMat"), 1, GL_FALSE, glm::value_ptr (projectionMatrix)); // Pass it to the GPU program
    
	glm::mat4 viewMatrix = camera.computeViewMatrix();
    vector<std::shared_ptr<Mesh>>::iterator mesh;
	for( mesh = meshGroup.begin(); mesh != meshGroup.end(); ++mesh ) {
		glm::mat4 modelMatrix = (*mesh)->computeTransformationMatrix();
		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
		glUniformMatrix4fv (glGetUniformLocation (program, "modelViewMat"), 1, GL_FALSE, glm::value_ptr (modelViewMatrix));
		
		(*mesh)->render();
	}
}

void clear (vector<std::shared_ptr<Mesh>> meshGroup) {
	glDeleteProgram (program);
	glfwDestroyWindow (window);

	vector<std::shared_ptr<Mesh>>::iterator mesh;
	for(mesh = meshGroup.begin(); mesh != meshGroup.end(); ++mesh) {
		(*mesh)->clear();
	}

	glfwTerminate ();
}

// Update any accessible variable based on the current time
void update (float currentTime) {
	// Animate any entity of the program here
	static const float initialTime = currentTime;
	float dt = currentTime - initialTime;
	// <---- Update here what needs to be animated over time ---->
}

int main (int argc, char ** argv) {
	std::shared_ptr<Mesh> mesh1 = Mesh::genCone(80);
	std::shared_ptr<Mesh> mesh2 = Mesh::genCone(80);
	std::shared_ptr<Mesh> mesh3 = Mesh::genCone(16);
	std::shared_ptr<Mesh> mesh4 = Mesh::genCone(80);
	std::shared_ptr<Mesh> mesh5 = Mesh::genCone(80);
	std::shared_ptr<Mesh> mesh6 = Mesh::genTorus(80);

	mesh1->set_translation_vector(glm::vec3(3.0, 0.0, 0.0));
	mesh2->set_translation_vector(glm::vec3(0.0, 0.0, 0.0));
	mesh3->set_translation_vector(glm::vec3(-3.0, 0.0, 0.0));
	mesh4->set_translation_vector(glm::vec3(-1.5, -2.0, 0.0));
	mesh5->set_translation_vector(glm::vec3(1.0, -1.0, -1.0));
	mesh5->set_rotation_x(-160.0);
	mesh5->set_rotation_y(20.0);
	mesh5->set_rotation_z(60.0);
	mesh6->set_translation_vector(glm::vec3(3.0, 0.0, 0.0));
	mesh6->set_scale(2.0);

	vector<std::shared_ptr<Mesh>> meshs = {
		mesh1, mesh2, mesh3, mesh4, mesh5, mesh6
	};

	camera.set_translation_vector(glm::vec3(0.0, 0.0, -10.0));
	camera.set_rotation_x(-90.0);

	init(meshs);

	while (!glfwWindowShouldClose(window)) {
		update (static_cast<float> (glfwGetTime()));
		render(meshs);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	clear(meshs);

	return EXIT_SUCCESS;
}

/*int main (int argc, char ** argv) {
	init (); // Your initialization code (user interface, OpenGL states, scene with geometry, material, lights, etc)
	while (!glfwWindowShouldClose (window)) {
		update (static_cast<float> (glfwGetTime ()));
		render ();
		glfwSwapBuffers (window);
		glfwPollEvents ();
	}
	clear ();
	return EXIT_SUCCESS;
}*/
