#pragma once

// general includes
#include <iostream>
#include <vector>
#include <cstdlib>

// glew includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// camera
#include "camera.h"

struct GLMesh {
	//vertex array
	GLuint vao;
	//vertex buffer
	GLuint vbo;
	//vertex buffer
	GLuint vbos[2];
	//indices of the mesh
	GLuint nIndices;

	//indices to draw
	std::vector<float> indices;
	//translation properties of the shape
	std::vector<float> properties;

	//physical properties of the shape
	float height;
	float length;
	float radius;
	float innerRadius;
	float numSides;

	// each shape gets a matrix object
	glm::mat4 scale;
	glm::mat4 xRotation;
	glm::mat4 yRotation;
	glm::mat4 zRotation;
	glm::mat4 rotation;
	glm::mat4 translation;
	glm::mat4 model;
	glm::vec2 uvScale;

	// texture information
	const char* texFile;
	GLuint textureID;

	//texture wrapping mode: repeat texture
	GLint textWrap = GL_REPEAT;

	class Mesh
	{
	};
};

struct GLightMesh {
	GLuint vao;
	GLuint vbo;
	GLuint nVertices;
};