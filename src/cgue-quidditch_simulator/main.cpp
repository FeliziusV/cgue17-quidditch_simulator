#include <GL\glew.h>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <GLFW\glfw3.h>
#include "scene\Cube.h"

#include "shader1.h"

using namespace cgue;

using namespace cgue::scene;


void init(GLFWwindow* window);

void update();
void draw();
void cleanup();
void glfw_on_error(int error_code, const char* desc);


std::unique_ptr<Shader> shader1;
std::unique_ptr<Cube> cube;

int main(int argc, char** argv) {


	//(1) init glfw
	if (!glfwInit())
	{
		std::cerr << "ERROR: Could not init glfw" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);

	}

	int width = 800;
	int height = 600;

	//commandline attributes for window size
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

	}

	//(2) set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//(3) open window
	auto window = glfwCreateWindow(width, height, "Hello, CGUE!", nullptr, nullptr);
	if (!window) {
		std::cerr << "ERROR: Could not open window" << std::endl;
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);


	//(4) init glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR: Could not initialize glew" << std::endl;
		glfwSetErrorCallback(glfw_on_error);

		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

/*
#if _DEBUG
	//Query the OpenGL function to register your callback function.
	PFNGLDEBUGMESSAGECALLBACKPROC _glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");
	PFNGLDEBUGMESSAGECALLBACKARBPROC _glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");
	PFNGLDEBUGMESSAGECALLBACKAMDPROC _glDebugMessageCallbackAMD = (PFNGLDEBUGMESSAGECALLBACKAMDPROC)wglGetProcAddress("glDebugMessageCallbackAMD");

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
*/

	//init everything i need
	init(window);


	glClearColor(0.35f, 0.36f, 0.43f, 0.3f);
	glViewport(0, 0, width, height);

	bool running = true;
	auto time = glfwGetTime();

	while (running && !glfwWindowShouldClose(window)) {

		auto time_new = glfwGetTime();
		auto time_delta = (float)(time_new - time);
		time = time_new;

		std::cout << "frametime: " << time * 1000 << "ms, " << 1.0 / time_delta << "FPS" << std::endl;

		glClear(GL_COLOR_BUFFER_BIT);

		update();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}

	}

	cleanup();
	glfwTerminate();


	return EXIT_SUCCESS;
}

void glfw_on_error(int error_code, const char* desc) {
	std::cerr << "(GLFW) " << desc << std::endl;
}

/*
static void APIENTRY DebugCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {
	std::string error = FormatDebugOutput(category, category, id, severity, message);
	std::cout << error << std::endl;
}

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}
*/


void init(GLFWwindow* window) {
	glfwSetWindowTitle(window, "CGUE Project");

	shader1 = std::make_unique<Shader>("Shader/basic.vert", "Shader/basic.frag");

	cube = std::make_unique<Cube>(glm::mat4(1.0f), shader1.get());

}
void update() { 
	cube->update();
}

void draw() {
	shader1->useShader();

	cube->draw();
}

void cleanup() {
	shader1.reset(nullptr);
	cube.reset(nullptr);
}

/*
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

*/