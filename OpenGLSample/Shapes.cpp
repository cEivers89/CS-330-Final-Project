#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

#include "Shapes.h"

using namespace std;

void Shapes::buildPyramid(GLMesh& mesh)
{
	// seed the srand function
	// NO LONGER NEEDED DUE TO USE OF TEXTURES NOW
	//srand(seed);
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };

	float h = mesh.height;
	// Minor change to below code to no longer include random colors
	mesh.indices = {
		// Vertex Positions		    // color coords 		  // Texture coords
		 0.0f,	h,	   0.0f,	color[0],	color[1],	color[2],	color[3],  0.625f, 1.0f,	//back side
		 0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	0.50f, 0.0f,
		-0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	0.75f, 0.0f,

		 0.0f,  h,	   0.0f,	color[0],	color[1],	color[2],	color[3],  0.875f, 1.0f,	//left side
		-0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	0.75f, 0.0f,
		-0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	 1.0f, 0.0f,

		 0.0f,  h,	   0.0f,	color[0],	color[1],	color[2],	color[3],  0.125f, 1.0f,	//front
		-0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	 0.0f, 0.0f,
		 0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	0.25f, 0.0f,

		 0.0f,  h,	   0.0f,	color[0],	color[1],	color[2],	color[3],  0.375f, 1.0f,	//right side
		 0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	0.25f, 0.0f,
		 0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	0.50f, 0.0f,

		-0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	1.0f, 1.0f,		//bottom back
		 0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	1.0f, 1.0f,
		-0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	1.0f, 1.0f,

		 0.5f, -0.0f, -0.5f,	color[0],	color[1],	color[2],	color[3],	1.0f, 1.0f,		//bottom front
		-0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	1.0f, 1.0f,
		 0.5f, -0.0f,  0.5f,	color[0],	color[1],	color[2],	color[3],	1.0f, 1.0f
	};

	buildMesh(mesh);
}


void Shapes::buildCone(GLMesh& mesh) {
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };

	float r = mesh.radius;
	float l = mesh.length;
	float s = mesh.numSides;

	constexpr float PI = 3.14f;
	const float sector = 2.0f * PI / s;
	const float texStep = 1.0f / s;
	float textureXLoc = 0.0f;

	vector<float> vertices;

	for (auto i = 1; i < s + 1; i++) {
		// triangle fan, bottom
		vertices.insert(vertices.end(), { 0.5f, 0.5f, 0.0f, color[0], color[1], color[2], color[3], 0.5f, 0.25f });		// center point; x, y, z, r, g, b, a, texture x, texture y
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),
										  0.5f + r * sin(i * sector),
										  0.0f,
										  color[0], color[1], color[2], color[3],
										  0.5f + (r * cos((i)*sector)),			// texture x
										  0.25f + (0.25f * sin((i)*sector))
			});	// first outer point
		vertices.insert(vertices.end(), { 0.5f + (r * cos((i + 1) * sector)),
										  0.5f + (r * sin((i + 1) * sector)),
										  0.0f,
										  color[0], color[1], color[2], color[3],
										  0.5f + (r * cos((i + 1) * sector)),
										  0.25f + (0.25f * sin((i + 1) * sector))
			});	// second outer point

		// side triangle + point
		vertices.insert(vertices.end(), { 0.5f + (r * cos(i * sector)),
										  0.5f + (r * sin(i * sector)),
										  0.0f,
										  color[0], color[1], color[2], color[3],
										  textureXLoc,
										  0.5f });
		vertices.insert(vertices.end(), { 0.5f + (r * cos((i + 1) * sector)),
										  0.5f + (r * sin((i + 1) * sector)),
										  0.0f,
										  color[0], color[1], color[2], color[3],
										  textureXLoc + texStep,
										  0.5f });
		vertices.insert(vertices.end(), { 0.5f , 0.5f , l , color[0], color[1], color[2], color[3], textureXLoc + (texStep / 2), 1.0f });		// origin, peak

		textureXLoc += texStep;

	}
	mesh.indices = vertices;
	vertices.clear();
	color.clear();

	buildMesh(mesh);
}

void Shapes::buildCube(GLMesh& mesh) {

	mesh.indices = {
		0.5f,	0.0f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,  0.25f,	0.5f,	// front left
	   -0.5f,	0.0f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	0.0f,	0.5f,
	   -0.5f,	1.0f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	0.0f,	1.0f,

		0.5f,	0.0f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,  0.25f,	0.5f,	// front right
		0.5f,	1.0f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,  0.25f,	1.0f,
	   -0.5f,	1.0f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	0.0f,	1.0f,


		0.5f,	0.0f,	0.5f,	1.0f,	0.0f,	0.0f,	1.0f,  0.25f,	0.5f,	// right front
		0.5f,	1.0f,	0.5f,	1.0f,	0.0f,	0.0f,	1.0f,  0.25f,	1.0f,
		0.5f,	1.0f,  -0.5f,	1.0f,	0.0f,	0.0f,	1.0f,   0.5f,	1.0f,

		0.5f,	0.0f,	0.5f,	1.0f,	0.0f,	0.0f,	1.0f,  0.25f,	0.5f,	// right back
		0.5f,	0.0f,  -0.5f,	1.0f,	0.0f,	0.0f,	1.0f,	0.5f,	0.5f,
		0.5f,	1.0f,  -0.5f,	1.0f,	0.0f,	0.0f,	1.0f,	0.5f,	1.0f,


		0.5f,	0.0f,  -0.5f,	0.0f,	0.0f,  -1.0f,	1.0f,	0.5f,	0.5f,	// back left
	   -0.5f,	0.0f,  -0.5f,	0.0f,	0.0f,  -1.0f,	1.0f,  0.75f,	0.5f,
	   -0.5f,	1.0f,  -0.5f,	0.0f,	0.0f,  -1.0f,	1.0f,  0.75f,	1.0f,

		0.5f,	0.0f,  -0.5f,	0.0f,	0.0f,  -1.0f,	1.0f,	0.5f,	0.5f,	// back right
		0.5f,	1.0f,  -0.5f,	0.0f,	0.0f,  -1.0f,	1.0f,	0.5f,	1.0f,
	   -0.5f,	1.0f,  -0.5f,	0.0f,	0.0f,  -1.0f,	1.0f,  0.75f,	1.0f,


	   -0.5f,	0.0f,	0.5f,  -1.0f,	0.0f,	0.0f,	1.0f,	1.0f,	0.5f,	// left back
	   -0.5f,	1.0f,	0.5f,  -1.0f,	0.0f,	0.0f,	1.0f,	1.0f,	1.0f,
	   -0.5f,	1.0f,  -0.5f,  -1.0f,	0.0f,	0.0f,	1.0f,  0.75f,	1.0f,

	   -0.5f,	0.0f,	0.5f,  -1.0f,	0.0f,	0.0f,	1.0f,	1.0f,	0.5f,	// left front
	   -0.5f,	0.0f,  -0.5f,  -1.0f,	0.0f,	0.0f,	1.0f,  0.75f,	0.5f,
	   -0.5f,	1.0f,  -0.5f,  -1.0f,	0.0f,	0.0f,	1.0f,  0.75f,	1.0f,


	   -0.5f,	1.0f,	0.5f,	0.0f,	1.0f,	0.0f,	1.0f,	0.0f,	0.0f,	// top left
	   -0.5f,	1.0f,  -0.5f,	0.0f,	1.0f,	0.0f,	1.0f,	0.0f,	0.5f,
		0.5f,	1.0f,	0.5f,	0.0f,	1.0f,	0.0f,	1.0f,  0.25f,	0.0f,

	   -0.5f,	1.0f,  -0.5f,	0.0f,	1.0f,	0.0f,	1.0f,	0.0f,	0.5f,	// top right
		0.5f,	1.0f,	0.5f,	0.0f,	1.0f,	0.0f,	1.0f,  0.25f,	0.0f,
		0.5f,	1.0f,  -0.5f,	0.0f,	1.0f,	0.0f,	1.0f,  0.25f,	0.5f,

	   -0.5f,	0.0f,	0.5f,	0.0f,  -1.0f,	0.0f,	1.0f,	0.0f,	0.0f,	// bottom left
	   -0.5f,	0.0f,  -0.5f,	0.0f,  -1.0f,	0.0f,	1.0f,	0.0f,	0.5f,
		0.5f,	0.0f,	0.5f,	0.0f,  -1.0f,	0.0f,	1.0f,  0.25f,	0.0f,

	   -0.5f,	0.0f,  -0.5f,	0.0f,  -1.0f,	0.0f,	1.0f,	0.0f,	0.5f,	// bottom right
		0.5f,	0.0f,   0.5f,	0.0f,  -1.0f,	0.0f,	1.0f,  0.25f,	0.0f,
		0.5f,	0.0f,  -0.5f,	0.0f,  -1.0f,	0.0f,	1.0f,  0.25f,	0.5f,
	};

	buildMesh(mesh);
}

void Shapes::buildCylinder(GLMesh& mesh) {
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };

	float r = mesh.radius;
	float l = mesh.length;
	float s = mesh.numSides;
	float h = mesh.height;


	constexpr float PI = 3.14f;
	const float sector = 2.0f * PI / s;

	vector<float> vertices;

	for (auto i = 1; i < s + 1; i++)
	{
		// triangle fan, bottom
		vertices.insert(vertices.end(), { 0.5f, 0.5f, 0.0f, color[0],	color[1], color[2],	color[3], 0.5f, 0.125f });			// origin (0.5, 0.5) works best for textures
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,			// x
										0.5f + r * sin(i * sector) ,			// y
										0.0f ,										// z
										color[0], color[1], color[2], color[3],						// color data r g b a
										0.5f + (r * cos((i)*sector)) ,		// texture x; adding the origin for proper alignment
										(0.125f + (0.125f * sin((i)*sector))) });		// texture y


		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										0.5f + r * sin((i + 1) * sector) ,
										0.0f ,
										color[0], color[1], color[2], color[3],						// color data r g b a
										0.5f + (r * cos((i + 1) * sector)) ,
										(0.125f + (0.125f * sin((i + 1) * sector))) });


	}

	for (auto i = 1; i < s + 1; i++)
	{
		// triangle fan, top
		vertices.insert(vertices.end(), { 0.5f, 0.5f, l, color[0], color[1], color[2], color[3], 0.5f, 0.875f });			// origin (0.5, 0.5) works best for textures
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,
										0.5f + r * sin(i * sector) ,
										l ,										// build this fan the 'l' value away from the other fan
										color[0], color[1], color[2], color[3],					// color data r g b a
										0.5f + (r * cos((i)*sector)) ,
										0.875f + (0.125f * sin((i)*sector)) });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										0.5f + r * sin((i + 1) * sector) ,
										l ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										0.5f + (r * cos((i + 1) * sector)) ,
										0.875f + (0.125f * sin((i + 1) * sector)) });
	}

	// since all side triangles have the same points as the fans above, the same calculations are used
	// to wrap the texture around the cylinder, the calculated points are used to determine which section of
	// the texture to clamp to the corresponding point.
	constexpr float x = 1.0f;
	float j = 1.0f / (s / x);	// for calculating texture location; change 'x' to increase or decrease how many times the texture wraps around the cylinder
	float k = 0.0f;				// for texture clamping

	// sides
	for (auto i = 1; i < s + 1; i++)
	{
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,
										0.5f + r * sin(i * sector) ,
										0.0f ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										k ,
										0.25f });
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,
										0.5f + r * sin(i * sector) ,
										l ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										k ,
										0.75f });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										0.5f + r * sin((i + 1) * sector) ,
										l ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										k + j ,
										0.75f });

		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										0.5f + r * sin((i + 1) * sector) ,
										l ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										k + j ,
										0.75f });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										0.5f + r * sin((i + 1) * sector) ,
										0.0f ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										k + j ,
										0.25f });
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,
										0.5f + r * sin(i * sector) ,
										0.0f ,
										color[0], color[1], color[2], color[3],					// color data r g b a
										k,
										0.25f });
		k += j;
	}

	mesh.indices = vertices;
	vertices.clear();
	buildMesh(mesh);

}

void Shapes::buildTorus(GLMesh& mesh) {
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };

	float ir = mesh.innerRadius;
	float r = mesh.radius;
	float h = mesh.height;
	float s = mesh.numSides;

	constexpr float PI = 3.14f;
	const float sector = 2.0f * PI / s;

	vector<float> vertices;

	for (auto i = 0; i < s; i++)
	{

		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;


		//BOTTOM OF HOLLOW CYLINDER
		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector),
										  0.0f,
										  0.5f + ir * sin(i * sector),
										 -color[0], -1.0f, -color[2], 1.0f,
										  0.5f + ((ir / r * 0.5f) * cos((i)*sector)),
									   (0.125f + ((ir / r * 0.125f) * sin((i)*sector))) });

		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),					// x
										  0.0f,												// y
										  0.5f + r * sin(i * sector),					// z
										  color[0], -1.0f, color[2], 1.0f,							// color data r g b a
										  0.5f + (0.5f * cos((i)*sector)),			// texture x; adding the origin for proper alignment
									   (0.125f + 0.125f * sin((i)*sector)) });		// texture y

		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  0.0f,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], -1.0f, -color[2], 1.0f,
										  0.5f + ((ir / r * 0.5f) * cos((i + 1) * sector)) ,
									   (0.125f + ((ir / r * 0.125f) * sin((i + 1) * sector))) });
		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  0.0f,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], -1.0f, -color[2], 1.0f,
										  0.5f + ((ir / r * 0.5f) * cos((i + 1) * sector)),
									   (0.125f + ((ir / r * 0.125f) * sin((i + 1) * sector))) });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector),
										  0.0f,
										  0.5f + r * sin((i + 1) * sector),
										  color[0], -1.0f, color[2], 1.0f,
										  0.5f + (0.5f * cos((i + 1) * sector)),
									   (0.125f + 0.125f * sin((i + 1) * sector)) });
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),
										  0.0f,
										  0.5f + r * sin(i * sector),
										  color[0], -1.0f, color[2], 1.0f,
										  0.5f + (0.5f * cos((i)*sector)),
									   (0.125f + (0.125f * sin((i)*sector))) });

	}

	for (auto i = 0; i < s; i++)
	{
		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;

		//TOP OF HOLLOW CYLINDER
		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector),
										  h,
										  0.5f + ir * sin(i * sector),
										 -color[0], 1.0f, -color[2], 1.0f,
										  0.5f + ((ir / r * 0.5f) * cos((i)*sector)),
									   (0.125f + ((ir / r * 0.125f) * sin((i)*sector))) });

		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),					// x
										  h,												// y
										  0.5f + r * sin(i * sector),					// z
										  color[0], 1.0f, color[2], 1.0f,							// color data r g b a
										  0.5f + (0.5f * cos((i)*sector)),			// texture x; adding the origin for proper alignment
									   (0.125f + 0.125f * sin((i)*sector)) });		// texture y

		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  h,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], 1.0f, -color[2], 1.0f,
									  	  0.5f + ((ir / r * 0.5f) * cos((i + 1) * sector)),
									   (0.125f + ((ir / r * 0.125f) * sin((i + 1) * sector))) });
		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  h,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], 1.0f, -color[2], 1.0f,
										  0.5f + ((ir / r * 0.5f) * cos((i + 1) * sector)),
									   (0.125f + ((ir / r * 0.125f) * sin((i + 1) * sector))) });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector),
										  h,
										  0.5f + r * sin((i + 1) * sector),
										  color[0], 1.0f, color[2], 1.0f,
										  0.5f + (0.5f * cos((i + 1) * sector)),
									   (0.125f + 0.125f * sin((i + 1) * sector)) });
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),
										  h,
										  0.5f + r * sin(i * sector),
										  color[0], 1.0f, color[2], 1.0f,
										  0.5f + (0.5f * cos((i)*sector)),
									   (0.125f + (0.125f * sin((i)*sector))) });

	}

	constexpr float x = 1.0f;
	float j = 1.0f / (s / x);	// for calculating texture location; change 'x' to increase or decrease how many times the texture wraps around the cylinder
	float k = 0.0f;				// for texture clamping


	// OUTSIDE SIDES OF HOLLOW CYLINDER
	for (auto i = 0; i < s; i++)
	{
		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;


		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),
										  0.0f,
										  0.5f + r * sin(i * sector),
										  color[0], 0.0f, color[2], color[3],					// color data r g b a
										  k,
										 0.25f });

		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),
										  h,
										  0.5f + r * sin(i * sector),
										  color[0], 0.0f, color[2], color[3],					// color data r g b a
										  k,
										 0.75f });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector),
										  h,
										  0.5f + r * sin((i + 1) * sector) ,
										  color[0], 0.0f, color[2], color[3],					// color data r g b a
										  k + j,
										 0.75f });

		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector),
										  h,
										  0.5f + r * sin((i + 1) * sector) ,
										  color[0], 0.0f, color[2], color[3],					// color data r g b a
										  k + j,
										 0.75f });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector),
										  0.0f,
										  0.5f + r * sin((i + 1) * sector),
										  color[0], 0.0f, color[2], color[3],					// color data r g b a
										  k + j,
										 0.25f });

		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector),
										0.0f ,
										0.5f + r * sin(i * sector),
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k,
										0.25f });
		k += j;
	}

	// INSIDE SIDES OF HOLLOW CYLINDER
	for (auto i = 0; i < s; i++)
	{
		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;


		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector),
										  0.0f,
										  0.5f + ir * sin(i * sector),
										  -color[0], 0.0f, -color[2], color[3],					// color data r g b a
										  k,
										 0.25f });

		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector),
										  h,
										  0.5f + ir * sin(i * sector),
										  -color[0], 0.0f, -color[2], color[3],					// color data r g b a
										  k,
										 0.75f });
		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  h,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], 0.0f, -color[2], color[3],					// color data r g b a
										 k + j,
										 0.75f });

		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  h,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], 0.0f, -color[2], color[3],					// color data r g b a
										  k + j,
										 0.75f });
		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector),
										  0.0f,
										  0.5f + ir * sin((i + 1) * sector),
										 -color[0], 0.0f, -color[2], color[3],					// color data r g b a
										 k + j,
										 0.25f });

		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector),
										  0.0f,
										  0.5f + ir * sin(i * sector),
										 -color[0], 0.0f, -color[2], color[3],					// color data r g b a
										  k,
										 0.25f });
		k += j;
	}

	mesh.indices = vertices;
	buildMesh(mesh);

}

void Shapes::buildBowl(GLMesh& mesh) {
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };

	float r = mesh.radius;
	float ir = mesh.radius * 0.75f;
	float h = mesh.height;
	float s = mesh.numSides;


	constexpr float PI = 3.14f;
	const float sector = 2.0f * PI / s;

	vector<float> vertices;

	for (auto i = 0; i < s; i++)
	{
		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;


		// triangle fan, bottom
		vertices.insert(vertices.end(), { 0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.125f });			// origin (0.5, 0.5) works best for textures
		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector) ,			// x
										0.0f ,										// y
										0.5f + ir * sin(i * sector) ,			// z
										color[0], -1.0f, color[2], 1.0f,						// color data r g b a
										0.5f + (0.5f * cos((i)*sector)) ,		// texture x; adding the origin for proper alignment
										(0.125f + (0.125f * sin((i)*sector))) });		// texture y


		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector) ,
										0.0f ,
										0.5f + ir * sin((i + 1) * sector) ,
										color[0], -1.0f, color[2], 1.0f,						// color data r g b a
										0.5f + (0.5f * cos((i + 1) * sector)) ,
										(0.125f + (0.125f * sin((i + 1) * sector))) });


	}

	for (auto i = 1; i < s + 1; i++)
	{

		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;
		// triangle fan, top
		vertices.insert(vertices.end(), { 0.5f, h, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.875f });			// origin (0.5, 0.5) works best for textures
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,
										h ,										// build this fan the 'l' value away from the other fan
										0.5f + r * sin(i * sector) ,
										color[0], 1.0f, color[2], 1.0f,					// color data r g b a
										0.5f + (0.5f * cos((i)*sector)) ,
										0.875f + (0.125f * sin((i)*sector)) });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										h ,
										0.5f + r * sin((i + 1) * sector) ,
										color[0], 1.0f, color[2], 1.0f,					// color data r g b a
										0.5f + (0.5f * cos((i + 1) * sector)) ,
										0.875f + (0.125f * sin((i + 1) * sector)) });
	}

	// since all side triangles have the same points as the fans above, the same calculations are used
	// to wrap the texture around the cylinder, the calculated points are used to determine which section of
	// the texture to clamp to the corresponding point.
	constexpr float x = 1.0f;
	float j = 1.0f / (s / x);	// calculating texture location
	float k = 0.0f;				// texture clamping

	// sides
	for (auto i = 0; i < s; i++)
	{
		float one = 0.5f + r * cos(i * sector);
		float two = 0.5f + r * sin(i * sector);

		one -= 0.5f;
		one *= 2.0f;

		two -= 0.5f;
		two *= 2.0f;

		color[0] = one;
		color[2] = two;


		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector) ,
										0.0f ,
										0.5f + ir * sin(i * sector) ,
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k ,
										0.25f });

		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,
										h ,
										0.5f + r * sin(i * sector) ,
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k ,
										0.75f });
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										h ,
										0.5f + r * sin((i + 1) * sector) ,
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k + j ,
										0.75f });

		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										h ,
										0.5f + r * sin((i + 1) * sector) ,
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k + j ,
										0.75f });
		vertices.insert(vertices.end(), { 0.5f + ir * cos((i + 1) * sector) ,
										0.0f ,
										0.5f + ir * sin((i + 1) * sector) ,
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k + j ,
										0.25f });

		vertices.insert(vertices.end(), { 0.5f + ir * cos(i * sector) ,
										0.0f ,
										0.5f + ir * sin(i * sector) ,
										color[0], 0.0f, color[2], color[3],					// color data r g b a
										k,
										0.25f });
		k += j;
	}

	mesh.indices = vertices;
	vertices.clear();
	buildMesh(mesh);
}

void Shapes::buildCircle(GLMesh& mesh) {
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };


	float r = mesh.radius;
	float l = mesh.length;
	float s = mesh.numSides;
	float h = mesh.height;

	constexpr float PI = 3.14f;
	const float sector = 2.0f * PI / s;

	vector<float> vertices;

	for (auto i = 1; i < s + 1; i++)
	{
		// triangle fan
		vertices.insert(vertices.end(), { 0.5f, 0.5f, 0.0f, color[0],	color[1], color[2],	color[3], 0.5f, 0.5f });		// origin (0.5, 0.5) works best for textures
		vertices.insert(vertices.end(), { 0.5f + r * cos(i * sector) ,		// x
										0.5f + r * sin(i * sector) ,		// y
										0.0f ,									// z
										color[0],	color[1], 1.0f,	color[3],				// color data r g b a
										0.5f + (r * cos((i)*sector)) ,	// texture x; adding the origin for proper alignment
										0.5f + (r * sin((i)*sector)) });	// texture y
		vertices.insert(vertices.end(), { 0.5f + r * cos((i + 1) * sector) ,
										0.5f + r * sin((i + 1) * sector) ,
										0.0f ,
										color[0],	color[1], 1.0f,	color[3],				// color data r g b a
										0.5f + (r * cos((i + 1) * sector)) ,
										0.5f + (r * sin((i + 1) * sector)) });
	}
	mesh.indices = vertices;
	vertices.clear();
	buildMesh(mesh);
}

void Shapes::buildPlane(GLMesh& mesh) {
	vector<float> color = { mesh.properties[0], mesh.properties[1], mesh.properties[2], mesh.properties[3] };

	mesh.indices = {
		-1.0f, 0.0f,-1.0f, color[0], color[1], color[2], color[3], 0.0f, 1.0f,	// 0
		 0.0f, 0.0f, 1.0f, color[0], color[1], color[2], color[3], 0.5f, 0.0f,	// 1
		-1.0f, 0.0f, 1.0f, color[0], color[1], color[2], color[3], 0.0f, 0.0f,	// 2

		-1.0f, 0.0f,-1.0f, color[0], color[1], color[2], color[3], 0.0f, 1.0f,	// 0
		 0.0f, 0.0f, 1.0f, color[0], color[1], color[2], color[3], 0.5f, 0.0f,	// 2
		 0.0f, 0.0f,-1.0f, color[0], color[1], color[2], color[3], 0.5f, 1.0f,	// 3

		 0.0f, 0.0f,-1.0f, color[0], color[1], color[2], color[3], 0.5f, 1.0f,	// 3
		 0.0f, 0.0f, 1.0f, color[0], color[1], color[2], color[3], 0.5f, 0.0f,	// 2
		 1.0f, 0.0f, 1.0f, color[0], color[1], color[2], color[3], 1.0f, 0.0f,	// 5

		 0.0f, 0.0f,-1.0f, color[0], color[1], color[2], color[3], 0.5f, 1.0f,	// 3
		 1.0f, 0.0f, 1.0f, color[0], color[1], color[2], color[3], 1.0f, 0.0f,	// 5
		 1.0f, 0.0f,-1.0f, color[0], color[1], color[2], color[3], 1.0f, 1.0f,	// 4
	};

	buildMesh(mesh);
}


// CUBE LIGHT TEMPLATE
void Shapes::createLightCube(GLightMesh& lightMesh) {

	// Position and Color data
	GLfloat vertices[] = {
		//Positions          //Normals
		// ------------------------------------------------------
		//Back Face          //Negative Z Normal  Texture Coords.
	 -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   //Front Face         //Positive Z Normal
	 -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

	  //Left Face          //Negative X Normal
	 -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Right Face         //Positive X Normal
	  0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Bottom Face        //Negative Y Normal
	 -0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	//Top Face           //Positive Y Normal
     -0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	lightMesh.nVertices = sizeof(vertices) / (sizeof(vertices[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	glGenVertexArrays(1, &lightMesh.vao);
	glBindVertexArray(lightMesh.vao);

	// Create 2 buffers - one for vertex data, one for indices
	glGenBuffers(1, &lightMesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, lightMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerUV)));
	glEnableVertexAttribArray(2);
}


// Moved to standalone function to maintain DRY methodology
void Shapes::buildMesh(GLMesh& mesh) {
	constexpr GLuint floatsPerVertex = 3;
	constexpr GLuint floatsPerColor = 4;
	constexpr GLuint floatsPerUV = 2;

	mesh.nIndices = mesh.indices.size() / (floatsPerVertex + floatsPerUV + floatsPerColor);

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer

	// use vector instead of array
	glBufferData(GL_ARRAY_BUFFER, mesh.indices.size() * sizeof(float), &mesh.indices.front(), GL_STATIC_DRAW);

	// Strides between vertex coordinates
	constexpr GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerColor);

	// Create Vertex Attribute Pointers
	// location
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// scale the object
	mesh.scale = glm::scale(glm::vec3(mesh.properties[4], mesh.properties[5], mesh.properties[6]));

	const glm::mat4 rot = glm::mat4(1.0f);

	// rotate the object 
	mesh.xRotation = glm::rotate(rot, glm::radians(mesh.properties[7]), glm::vec3(mesh.properties[8], mesh.properties[9], mesh.properties[10]));
	mesh.yRotation = glm::rotate(rot, glm::radians(mesh.properties[11]), glm::vec3(mesh.properties[12], mesh.properties[13], mesh.properties[14]));
	mesh.zRotation = glm::rotate(rot, glm::radians(mesh.properties[15]), glm::vec3(mesh.properties[16], mesh.properties[17], mesh.properties[18]));

	// move the object
	mesh.translation = glm::translate(glm::vec3(mesh.properties[19], mesh.properties[20], mesh.properties[21]));

	mesh.model = mesh.translation * mesh.xRotation * mesh.zRotation * mesh.yRotation * mesh.scale;
	// scales the texture
	//mesh.uvScale = glm::vec2(mesh.properties[22], mesh.properties[23]); 
	mesh.uvScale = glm::vec2(2.0f, 2.0f);
}