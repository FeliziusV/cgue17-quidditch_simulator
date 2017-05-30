#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <FreeImage\FreeImage.h>


#include "Game.h"
#include "scene\Cube.h"
#include "scene\Texture.h"
#include "shader.h"
#include "scene\Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace cgue;
using namespace cgue::scene;

//Prototypen
void glfw_on_error(int error_code, const char* desc);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
bool keys[1024] = {0};

std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, 
	GLenum severity, const char* msg);
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, 
	GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
void APIENTRY DebugCallbackAMD(GLuint id, GLenum category, 
	GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

std::unique_ptr<Game> game;
std::unique_ptr<Shader> shader1;
std::unique_ptr<Cube> cube;
std::unique_ptr<Texture> texture;


int main(int argc, char** argv) {


	if (!glfwInit())
	{
		std::cerr << "ERROR: Could not init glfw" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);

	}

	int width = 800;
	int height = 600;

	bool fullscreen = false;

	if (argc >= 3) {
		std::cout << "Your are executing '" << argv[0] << "'" << std::endl;

		if ((std::stringstream(argv[1]) >> width).fail()) {
			std::cerr << "ERROR: Could not parse first command-line-argument as integer." << std::endl;
			exit(EXIT_FAILURE);
		}
		if ((std::stringstream(argv[2]) >> height).fail()) {
			std::cerr << "ERROR: Could not parse second command-line-argument as integer." << std::endl;
			exit(EXIT_FAILURE);
		}
		if (argc >= 4) {
			if ((std::stringstream(argv[3]) >> fullscreen).fail()) {
				std::cerr << "ERROR Could not parse third command-line-argument as boolean." << std::endl;
				system("PAUSE");
				exit(EXIT_FAILURE);
			}
		}
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWmonitor * monitor = nullptr;
	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
		auto refresh_rate = 60;
		glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
	}

	auto window = glfwCreateWindow(width, height, "Hello, CGUE!", monitor, nullptr);
	if (!window) {
		std::cerr << "ERROR: Could not open window" << std::endl;
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);


	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR: Could not initialize glew" << std::endl;
		glfwSetErrorCallback(glfw_on_error);

		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, keyCallback);

	//glDebugMessageCallback(debug_callback_proc, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);



#if _DEBUG
	//Query the OpenGL function to register your callback function.
	PFNGLDEBUGMESSAGECALLBACKPROC _glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)glfwGetProcAddress("glDebugMessageCallback");
	PFNGLDEBUGMESSAGECALLBACKARBPROC _glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)glfwGetProcAddress("glDebugMessageCallbackARB");
	PFNGLDEBUGMESSAGECALLBACKAMDPROC _glDebugMessageCallbackAMD = (PFNGLDEBUGMESSAGECALLBACKAMDPROC)glfwGetProcAddress("glDebugMessageCallbackAMD");

	// Register your callback function.
	if (_glDebugMessageCallback != NULL) {
		_glDebugMessageCallback(DebugCallback, NULL);
	}
	else if (_glDebugMessageCallbackARB != NULL) {
		_glDebugMessageCallbackARB(DebugCallback, NULL);
	}
	else if (_glDebugMessageCallbackAMD != NULL) {
		_glDebugMessageCallbackAMD(DebugCallbackAMD, NULL);
	}

	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. This capability is not defined in the AMD
	// version.
	if ((_glDebugMessageCallback != NULL) || (_glDebugMessageCallbackARB != NULL)) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
#endif



	game = std::make_unique<Game>();
	game->init(window);
	game->gameLoop();

	game.reset(nullptr);

	return EXIT_SUCCESS;
}

void glfw_on_error(int error_code, const char* desc) {
	std::cerr << "(GLFW) " << desc << std::endl;
}


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (action == GLFW_PRESS){
		if (key == GLFW_KEY_LEFT) {
			game->camera->left = true;
		}
		if (key == GLFW_KEY_RIGHT) {
			game->camera->right = true;
		}
		if (key == GLFW_KEY_UP) {
			game->camera->up = true;
		}
		if (key == GLFW_KEY_DOWN) {
			game->camera->down = true;
		}
		if (key == GLFW_KEY_LEFT_SHIFT) {
			game->camera->shift = true;
		}
		if (key == GLFW_KEY_SPACE) {
			game->camera->space = true;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) {
			game->camera->left = false;
		}
		if (key == GLFW_KEY_RIGHT) {
			game->camera->right = false;
		}
		if (key == GLFW_KEY_UP) {
			game->camera->up = false;
		}
		if (key == GLFW_KEY_DOWN) {
			game->camera->down = false;
		}
		if (key == GLFW_KEY_LEFT_SHIFT) {
			game->camera->shift = false;
		}
		if (key == GLFW_KEY_SPACE) {
			game->camera->space = false;
		}
	}
	/*
	switch (action) {
	case GLFW_PRESS:
		switch (key) {
		case GLFW_KEY_LEFT:
			game->camera->inputPress("LEFT");
			break;

		case GLFW_KEY_RIGHT:
			game->camera->inputPress("RIGHT");
			break;

		case GLFW_KEY_UP:
			game->camera->inputPress("UP");
			break;

		case GLFW_KEY_DOWN:
			game->camera->inputPress("DOWN");
			break;

		case GLFW_KEY_LEFT_SHIFT:
			keys[key] = true;
			game->camera->inputPress("SHIFT");
			break;

		case GLFW_KEY_SPACE:
			keys[key] = true;
			game->camera->inputPress("SPACE");
			break;

		default:
			break;
		}
	case GLFW_RELEASE:
		switch (key) {
		case GLFW_KEY_LEFT:
			game->camera->inputRelease("LEFT");
			break;

		case GLFW_KEY_RIGHT:
			game->camera->inputRelease("RIGHT");
			break;

		case GLFW_KEY_UP:
			game->camera->inputRelease("UP");
			break;

		case GLFW_KEY_DOWN:
			game->camera->inputRelease("DOWN");
			break;

		case GLFW_KEY_LEFT_SHIFT:
			game->camera->inputRelease("SHIFT");
			break;

		case GLFW_KEY_SPACE:
			game->camera->inputRelease("SPACE");
			break;

		default:
			break;
		}
	
	case GLFW_REPEAT:

		switch (key) {
		case GLFW_KEY_LEFT:
			game->camera->rotate(-100.0f * game->time_delta);

			break;

		case GLFW_KEY_RIGHT:
			game->camera->rotate(100.0f * game->time_delta);
			break;

		case GLFW_KEY_UP:
			game->camera->accelerate();
			break;

		case GLFW_KEY_DOWN:
			game->camera->decelerate();
			break;

		case GLFW_KEY_LEFT_SHIFT:
			game->camera->rise(-100.0f * game->time_delta);
			break;

		case GLFW_KEY_SPACE:
			game->camera->rise(100.0f * game->time_delta);
			break;

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;

		default:
			break;
		}

	}
	*/

}



static void APIENTRY DebugCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {
	std::string error = FormatDebugOutput(category, category, id, severity, message);
	std::cout << error << std::endl;
}

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}




static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	// The AMD variant of this extension provides a less detailed classification of the error,
	// which is why some arguments might be "Unknown".
	switch (source) {
	case GL_DEBUG_CATEGORY_API_ERROR_AMD:
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_CATEGORY_APPLICATION_AMD:
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}

