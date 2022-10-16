/*	File is used to place the created object in the world
	utilizing the parameters below. This makes the program
	far more modular and allows at least a slightly easier way 
	to create and place objects. */

#include "WorldScene.h"
#include "Shapes.h"
#include "Mesh.h"

using namespace std;

void WorldScene::buildWorldScene(vector<GLMesh>& scene)
{
	// BEGIN BOWL
	GLMesh bowl;
	bowl.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		90.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	0.0f,
		1.0f,	1.0f
	};
	bowl.radius = 2.0f;
	bowl.height = 1.5f;
	bowl.numSides = 250.0f;
	bowl.texFile = "textures\\teal-plastic.jpg";
	Shapes::buildBowl(bowl);
	scene.push_back(bowl);

	GLMesh bowlTop;
	bowlTop.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		90.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		0.0f,	1.5f,	0.0f,
		1.0f,	1.0f
	};
	bowlTop.radius = 2.01f;
	bowlTop.innerRadius = 2.0f;
	bowlTop.height = 0.2f;
	bowlTop.numSides = 250.0f;
	bowlTop.texFile = "textures\\teal-plastic.jpg";
	Shapes::buildTorus(bowlTop);
	scene.push_back(bowlTop);

	GLMesh cereal;
	cereal.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		90.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	20.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		0.0f,	1.51f,	-1.0f,
		1.0f,	1.0f
	};
	cereal.radius = 2.0f;
	cereal.innerRadius = 1.8f;
	cereal.height = 0.5f;
	cereal.numSides = 250.0f;
	cereal.texFile = "textures\\cereal.jpg";
	Shapes::buildCircle(cereal);
	scene.push_back(cereal);
	// END BOWL

	// BEGIN HERSHEY'S KISS
	GLMesh kiss;
	kiss.properties = {
		1.0f,	1.0f,  1.0f, 1.0f,
		0.8f,	0.8f,  0.8f,
		-90.0f,	1.0f,  0.0f, 0.0f,
		0.0f,	0.0f,  1.0f, 0.0f,
		0.0f,	0.0f,  0.0f, 1.0f,
		-2.5f,	0.0f, -1.5f,
		1.0f,	1.0f
	};
	kiss.length = 0.8f;
	kiss.radius = 0.5;
	kiss.numSides = 250.0f;
	kiss.texFile = "textures\\silver-foil.jpg";
	Shapes::buildCone(kiss);
	scene.push_back(kiss);

	GLMesh kissFlag;
	kissFlag.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		0.7f,	0.7f,	0.05f,
		60.0f,	1.0f,	0.0f,	0.0f,
		90.0f,	0.0f,	1.0f,	0.0f,
		180.0f,	0.0f,	0.0f,	1.0f,
		-2.1f,	0.82f,	-2.02f,
		1.0f,	1.0f
	};
	kissFlag.texFile = "textures\\kiss_flag.png";
	Shapes::buildPlane(kissFlag);
	scene.push_back(kissFlag);
	// END HERSHEY'S KISS


	// BEGIN COFFEE MUG
	GLMesh coffeeMug;
	coffeeMug.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		2.0f,	2.0f,	2.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		-90.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		-6.0f,	0.0f,	-1.5f,
		1.0f,	1.0f
	};
	coffeeMug.innerRadius = 0.45f;
	coffeeMug.radius = 0.5f;
	coffeeMug.height = 1.0f;
	coffeeMug.numSides = 250.0f;
	coffeeMug.texFile = "textures\\coffee_mug.jpg";
	Shapes::buildTorus(coffeeMug);
	scene.push_back(coffeeMug);

	GLMesh coffeeHandle;
	coffeeHandle.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		2.0f,	2.0f,	2.0f,
		90.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		-9.2f,	2.0f,	-0.8f,
		1.0f,	1.0f
	};
	coffeeHandle.innerRadius = 0.3f;
	coffeeHandle.radius = 0.4f;
	coffeeHandle.height = 0.1f;
	coffeeHandle.numSides = 250.0f;
	coffeeHandle.texFile = "textures\\coffee_mug.jpg";
	Shapes::buildTorus(coffeeHandle);
	scene.push_back(coffeeHandle);

	GLMesh coffee;
	coffee.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		2.0f,	2.0f,	2.0f,
		90.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	20.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		-8.0f,	1.9f,	-1.5f,
		1.0f,	1.0f
	};
	coffee.radius = 0.45f;
	coffee.numSides = 250.0f;
	coffee.texFile = "textures\\coffee.jpg";
	Shapes::buildCircle(coffee);
	scene.push_back(coffee);
	// END COFFEE MUG

	// BEGIN HAMMER
	GLMesh hammerTop;
	hammerTop.properties = {
		1.0f,	1.0f,	1.0f,	1.0f,
		1.4f,	1.4f,	2.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		100.0f, 0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
		-3.5f,	0.0f,	0.0f,
		1.0f,	1.0f
	};
	hammerTop.texFile = "textures\\metal.jpg";
	Shapes::buildCube(hammerTop);
	scene.push_back(hammerTop);

	GLMesh hammerHandle;
	hammerHandle.properties = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-180.0f, 1.0f, 0.0f, 0.0f,
		-8.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		-4.0f, 1.1f, 0.0f,
		1.0f, 1.0f
	};
	hammerHandle.length = 4.0f;
	hammerHandle.radius = 0.2f;
	hammerHandle.numSides = 250.0f;
	hammerHandle.texFile = "textures\\wood.jpg";
	Shapes::buildCylinder(hammerHandle);
	scene.push_back(hammerHandle);

	// BEGIN PLANE
	GLMesh plane;
	plane.properties = {
		1.0f, 1.0f, 1.0f, 1.0f,				// color r, g, b a
		8.0f, 2.0f, 8.0f,					// scale x, y, z
		0.0f, 1.0f, 0.0f, 0.0f,				// x amount of rotation, rotate x, y, z
		0.0f, 0.0f, 1.0f, 0.0f,				// y amount of rotation, rotate x, y, z
		0.0f, 0.0f, 0.0f, 1.0f,				// z amount of rotation, rotate x, y, z
		-3.0f, 0.0f, 0.0f,					// translate x, y, z
		2.0f, 2.0f
	};

	plane.texFile = "textures\\stone-texture.jpg";
	Shapes::buildPlane(plane);
	scene.push_back(plane);
	// END PLANE
}