#include <iostream>
#include <memory>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>


void init(GLFWwindow* window);

void update();
void draw();
void cleanup();
void glfw_on_error(int error_code, const char* desc);


int main() {


	//(1) init glfw
	if (!glfwInit())
	{
		std::cerr << "ERROR: Could not init glfw" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);

	}

	const int widht = 800;
	const int height = 600;

	//(2) set window hints
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//(3) open window
	auto window = glfwCreateWindow(widht, height, "Hello, CGUE!", nullptr, nullptr);
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

	//init everything i need
	init(window);


	glClearColor(0.35f, 0.36f, 0.43f, 0.3f);
	glViewport(0, 0, widht, height);

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

void init(GLFWwindow* window) {
	glfwSetWindowTitle(window, "CGUE Project");

}
void update() {  }

void draw() {

}

void cleanup() {

}

