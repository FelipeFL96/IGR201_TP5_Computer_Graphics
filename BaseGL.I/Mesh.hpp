#ifndef _MESH_H
#define _MESH_H

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <vector>
#include <memory>

#include "Transform.hpp"

class Mesh : public Transform {
public:

	static std::shared_ptr<Mesh> genSphere (size_t resolution = 16);
	static std::shared_ptr<Mesh> genCone (size_t resolution = 16);
	static std::shared_ptr<Mesh> genCylinder (size_t resolution = 16);
	static std::shared_ptr<Mesh> genCube (size_t resolution = 16);
	static std::shared_ptr<Mesh> genTorus (size_t resolution = 16);

    void init();
	void render();
	void clear();

private:

	void initGPUGeometry();

	std::vector<float> vertexPositions;
	std::vector<float> vertexColors;
	std::vector<unsigned int> triangleIndices;

	GLuint posVbo; 
	GLuint colVbo; 
	GLuint ibo;
	GLuint vao;
};

#endif //_MESH_H