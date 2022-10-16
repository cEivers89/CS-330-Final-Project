#include "Shapes.h"
#include "Mesh.h"
#include "WorldScene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// shader program macros
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

using namespace std;

//window title
const char* const WINDOW_TITLE = "Final Project";

//window width, height
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

// Light mesh
GLightMesh ambientLight;
GLightMesh keyLight;

//main window
GLFWwindow* window = nullptr;

//shader program
GLuint shader;
GLuint lightShader;
GLuint shadowShader;

// scene vector for drawing shapes
vector<GLMesh> scene;

// variable to handle ortho change
bool perspective = false;


// camera
Camera camera(glm::vec3(-6.0f, 6.0f, -7.0f), glm::vec3(0.0f, 1.0f, 0.0f), 53.0f, -23.0f);
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// Spotlight Color, position, and scale
glm::vec3 spotLightColor(1.0f, 1.0f, 1.0f);
glm::vec3 spotLightPos(2.5f, 6.0f, -1.5f);
glm::vec3 spotLightScale(0.1f);

// Key light color, position, and scale
glm::vec3 keyLightColor(0.0f, 0.0f, 0.0f);
glm::vec3 keyLightPos(-2.5f, 4.0f, -1.5f);
glm::vec3 keyLightScale(0.0f);

bool spotLightOrbit = false; // Used to make spotlight circle around object

//initialize program
bool initializeProgram(int, char* [], GLFWwindow** window);
//set window size
void resizeWindow(GLFWwindow* window, int width, int height);
//process user input
void input(GLFWwindow* window);
//render graphics
void renderGraphics(vector<GLMesh> scene);
//compile shaders
bool shaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programID);

//free up memory on close
void destroyMesh(GLMesh& mesh);
void destroyShader(GLuint programID);
void destroyTexture(GLuint textureID);

// keyboard and mouse input functions
void mousePosition(GLFWwindow* window, double xpos, double ypos);
void mouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void mouseButton(GLFWwindow* window, int button, int action, int mods);

// texture create
bool createTexture(const char* fileName, GLuint& textureID);

// TODO: PUT SHADER PROGRAM BACK INTO OWN FILE
// BEGIN SHADER SOURCE
// Vertex Shader Source Code
const GLchar* vertexShaderSource = GLSL(440,
	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
	layout(location = 1) in vec3 normal; // VAP position 1 for normals
	layout(location = 2) in vec2 textureCoordinate;

	out vec3 vertexNormal; // For outgoing normals to fragment shader
	out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
	out vec2 vertexTextureCoordinate;

	//Uniform / Global variables for the  transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vertexFragmentPos = vec3(model * vec4(position, 1.0f));
	vertexNormal = mat3(transpose(inverse(model))) * normal;
	vertexTextureCoordinate = textureCoordinate;
}
);

// Fragment Shader Source Code
const GLchar* fragmentShaderSource = GLSL(440,
	in vec3 vertexFragmentPos;
	in vec3 vertexNormal;
	in vec2 vertexTextureCoordinate; // for texture coordinates

	out vec4 fragmentColor;

	uniform vec3 objectColor;
	uniform vec3 lightColor;
	uniform vec3 kLightColor;
	uniform vec3 lightPos;
	uniform vec3 kLightPos;
	uniform vec3 viewPosition;

	uniform sampler2D uTexture;
	uniform vec2 uvScale;

void main()
{
	//Calculate Ambient lighting
	float spotStrength = 0.2f; // Set ambient or global lighting strength
	float keyStrength = 0.3f; // Set ambient or global lighting strength
	vec3 spot = spotStrength * lightColor; // Generate ambient light color
	vec3 key = keyStrength * kLightColor;

	//Calculate Diffuse lighting
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance between light source and fragments/pixels
	vec3 keyLightDirection = normalize(kLightPos - vertexFragmentPos); // Calculate distance  between light source and fragments/pixels

	float impact = max(dot(norm, lightDirection), 0.0); // Calculate diffuse impact by generating dot product of normal and light
	float keyImpact = max(dot(norm, keyLightDirection), 0.0); // Calculate diffuse impact by generating dot product of normal and light

	vec3 diffuse = impact * lightColor; // Generate diffuse light color
	vec3 keyDiffuse = keyImpact * kLightColor;

	//Calculate Specular lighting*/
	float specularIntensity = 0.4f; // Set specular light strength
	float highlightSize = 16.0f; // Set specular highlight size
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
	//Calculate specular component
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * lightColor;
	vec3 keySpecular = specularIntensity * specularComponent * kLightColor;

	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

	// Calculate phong result
	vec3 phong = (spot + key + diffuse + specular) * textureColor.xyz;

	fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU

}
);

// Light shader 
const GLchar* lampVertexShaderSource = GLSL(440,
	layout(location = 0) in vec3 position;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
}
);

// Light fragment shader
const GLchar* lampFragmentShaderSource = GLSL(440,

	out vec4 fragmentColor;

void main() {
	fragmentColor = vec4(1.0f); // Sets color to white
}
);


// END SHADER SOURCE

void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}

// BEGIN MAIN
int main(int argc, char* argv[])
{

	//check if initialized correctly
	if (!initializeProgram(argc, argv, &window))
		return EXIT_FAILURE;

	WorldScene::buildWorldScene(scene);

	//build shader 
	if (!shaderProgram(vertexShaderSource, fragmentShaderSource,
		shader))
		return EXIT_FAILURE;

	for (auto& shape : scene)
	{
		if (!createTexture(shape.texFile, shape.textureID))
		{
			cout << "Failed to load texture " << shape.texFile << endl;
			return EXIT_FAILURE;

		}

		if (!shaderProgram(vertexShaderSource, fragmentShaderSource, shader))
			return EXIT_FAILURE;
	}

	if (!shaderProgram(lampVertexShaderSource, lampFragmentShaderSource, lightShader))
		return EXIT_FAILURE;

	// Create light object
	Shapes::createLightCube(ambientLight);
	Shapes::createLightCube(keyLight);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	glUseProgram(shader);
	// We set the texture as texture unit 0
	glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

	//rendering loop
	//keep checking if window has closed
	while (!glfwWindowShouldClose(window))
	{
		//bg color of window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//process user input
		input(window);

		//render frame
		renderGraphics(scene);

		glfwPollEvents();
	}

	//clean up
	for (auto& shape : scene)
	{
		destroyMesh(shape);
	}

	scene.clear();

	destroyShader(shader);

	//exit with success!
	exit(EXIT_SUCCESS);

} // END MAIN


// initialize and build shaders
bool initializeProgram(int argc, char* argv[], GLFWwindow** window)
{
	//initialize GLFW, GLEW, create window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	//use constants for vars
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	//window fails to be created
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, resizeWindow);
	glfwSetCursorPosCallback(*window, mousePosition);
	glfwSetScrollCallback(*window, mouseScroll);
	glfwSetMouseButtonCallback(*window, mouseButton);
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	return true;
}


bool shaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programID)
{
	//error reporting
	int success = 0;
	char infoLog[512];

	//create shader program object
	programID = glCreateProgram();

	//create vertex and frag shader obj
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//get shader source
	glShaderSource(vertexShaderID, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderID, 1, &fragShaderSource, NULL);

	//compile vertex shader
	glCompileShader(vertexShaderID);

	//check for errors
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	//compile frag shader
	glCompileShader(fragmentShaderID);

	//check for errors
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	//shaders compiled, attach to shader program object from above
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//link the program object
	glLinkProgram(programID);

	//check for linking errors
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(programID);

	return true;
}


// process user input and windows changes
void input(GLFWwindow* window)
{


	// exit program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// draw lines
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// fill shapes
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	// Camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}

	// Change perspective
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		perspective = false;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		perspective = true;

	// Light movement
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		spotLightPos.x -= 0.005f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		spotLightPos.x += 0.005f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		spotLightPos.z -= 0.005f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		spotLightPos.z += 0.005f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		spotLightPos.y -= 0.005f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		spotLightPos.y += 0.005f;

	// Change key light using keys R, G, and B
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		keyLightColor.r += 0.001f;
		if (keyLightColor.r > 1.0f) {
			keyLightColor.r = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		keyLightColor.g += 0.001f;
		if (keyLightColor.g > 1.0f) {
			keyLightColor.g = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		keyLightColor.b += 0.001f;
		if (keyLightColor.b > 1.0f) {
			keyLightColor.b = 0.0f;
		}
	}

	// Turn key light off
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
		keyLightColor.r = 0.0f;
		keyLightColor.g = 0.0f;
		keyLightColor.b = 0.0f;
	}
	// Turn on key light
	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
		keyLightColor.r = 1.0f;
		keyLightColor.g = 1.0f;
		keyLightColor.b = 1.0f;
	}

	// Stop and start spot light orbit
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !spotLightOrbit) {
		spotLightOrbit = true;
	} 
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && spotLightOrbit) {
		spotLightOrbit = false;
	}
}


void resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mousePosition(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


void mouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	// change camera speed by mouse scroll
	camera.ProcessMouseScroll(yoffset);
}


void mouseButton(GLFWwindow* window, int button, int action, int mods)
{

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		if (action == GLFW_PRESS)
			cout << "Left Pressed" << endl;
		else
			cout << "Left Released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		if (action == GLFW_PRESS)
			cout << "Middle Pressed" << endl;
		else
			cout << "Middle Released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (action == GLFW_PRESS)
			cout << "Right Pressed" << endl;
		else
			cout << "Right Released" << endl;
	}
	break;

	default:
		cout << "Unknown/Unhandled mouse button" << endl;
		break;
	}
}


// render the scene
void renderGraphics(vector<GLMesh> scene)
{
	// Borrowed from tutorial. Spot light circles around the scene
	constexpr float angularVelocity = glm::radians(45.0f);
	if (spotLightOrbit) {
		glm::vec4 newPosition = glm::rotate(angularVelocity * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(spotLightPos, 1.0f);
		spotLightPos.x = newPosition.x;
		spotLightPos.y = newPosition.y;
		spotLightPos.z = newPosition.z;
	}

	// Enable z-depth
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// transform the camera (x, y, z)
	glm::mat4 view = camera.GetViewMatrix();

	// create perspective projection (fov, aspect ratio, near plane, far plane)
	glm::mat4 projection;
	if (!perspective)
	{
		// press 'p' for perspective (default)
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	}
	else
		// press 'o' for ortho
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

	// loop to draw each shape individually
	for (auto i = 0; i < scene.size(); ++i)
	{
		auto mesh = scene[i];

		// Activate vbo's within each mesh's vao
		glBindVertexArray(mesh.vao);
		// Set shader
		glUseProgram(shader);

		// gets and passes transform matrices to shader prgm
		GLint modelLocation = glGetUniformLocation(shader, "model");
		GLint viewLocation = glGetUniformLocation(shader, "view");
		GLint projLocation = glGetUniformLocation(shader, "projection");

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mesh.model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

		// References matrix uniforms from the shape shader program for shape color, light color, light pos, and camera pos
		GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");

		// Spotlight
		GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shader, "lightPos");

		// Key light
		GLint keyLightColorLoc = glGetUniformLocation(shader, "kLightColor");
		GLint keyLightPosLoc = glGetUniformLocation(shader, "kLightPos");

		// Camera view
		GLint viewPositionLoc = glGetUniformLocation(shader, "viewPos");

		// Pass color, light, and camera data to shader
		glUniform3f(objectColorLoc, mesh.properties[0], mesh.properties[1], mesh.properties[2]);

		// Spot light
		glUniform3f(lightColorLoc, spotLightColor.r, spotLightColor.g, spotLightColor.b);
		glUniform3f(lightPosLoc, spotLightPos.x, spotLightPos.y, spotLightPos.z);

		// Key Light
		glUniform3f(keyLightColorLoc, keyLightColor.r, keyLightColor.g, keyLightColor.b);
		glUniform3f(keyLightPosLoc, keyLightPos.x, keyLightPos.y, keyLightPos.z);

		const glm::vec3 cameraPos = camera.Position;
		glUniform3f(viewPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		GLint uvScaleLoc = glGetUniformLocation(shader, "uvScale");
		glUniform2fv(uvScaleLoc, 1, glm::value_ptr(mesh.uvScale));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.textureID);

		// Draw the shape
		glDrawArrays(GL_TRIANGLES, 0, mesh.nIndices);
	}

	// Draw spot light
	glUseProgram(lightShader);
	glBindVertexArray(ambientLight.vao);
	// Light location and scale
	glm::mat4 model = glm::translate(spotLightPos) * glm::scale(spotLightScale);

	// Matrix uniforms from light shader
	GLint modelLoc = glGetUniformLocation(lightShader, "model");
	GLint viewLoc = glGetUniformLocation(lightShader, "view");
	GLint projLoc = glGetUniformLocation(lightShader, "projection");

	// Matrix data
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw light
	glDrawArrays(GL_TRIANGLES, 0, ambientLight.nVertices);

	// Draw key light
	glUseProgram(lightShader);
	glBindVertexArray(keyLight.vao);

	// Light location and scale
	model = glm::translate(keyLightPos) * glm::scale(keyLightScale);

	modelLoc = glGetUniformLocation(lightShader, "model");
	viewLoc = glGetUniformLocation(lightShader, "view");
	projLoc = glGetUniformLocation(lightShader, "projection");

	glDrawArrays(GL_TRIANGLES, 0, keyLight.nVertices);

	// deactivate vao
	glBindVertexArray(0);
	glUseProgram(0);

	// swap front and back buffers
	glfwSwapBuffers(window);

}

// clean-up methods
void destroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(1, &mesh.vbo);
}
void destroyShader(GLuint programId)
{
	glDeleteProgram(programId);
}


bool createTexture(const char* fileName, GLuint& textureID)
{
	int width, height, channels;
	unsigned char* image = stbi_load(fileName, &width, &height, &channels, 0);
	if (image)
	{
		flipImageVertically(image, width, height, channels);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			cout << "Not implemented to handle image with " << channels << " channels" << endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	// Error loading the image
	return false;
}
void destroyTexture(GLuint textureID)
{
	glGenTextures(1, &textureID);
}