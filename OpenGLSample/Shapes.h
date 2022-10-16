#pragma once

#include "Mesh.h"

using namespace std;

class Shapes
{
public:
	// Useable shapes
	static void buildPyramid(GLMesh& mesh);

	static void buildCone(GLMesh& mesh);

	static void buildCube(GLMesh& mesh);

	static void buildCylinder(GLMesh& mesh);

	static void buildTorus(GLMesh& mesh);

	static void buildCircle(GLMesh& mesh);

	static void buildPlane(GLMesh& mesh);

	static void buildBowl(GLMesh& mesh);

	// Light
	static void createLightCube(GLightMesh& mesh);

	// Build meshes
	static void buildMesh(GLMesh& mesh);
};