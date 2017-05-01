#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <FreeImage\FreeImage.h>


#include "Game.h"


using namespace cgue;
using namespace cgue::scene;

Game::Game() {


}

void Game::init(GLFWwindow* window)
{

	this->window = window;
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowTitle(this->window, "CGUE Project");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "ERROR: Could not init SDL Video" << std::endl;
		system("PAUSE");
		//exit(EXIT_FAILURE);
	}
	int quit = 0;

	//cube 1
	//******************************
	shader = std::make_unique<Shader>("Shader/basic.vert", "Shader/basic.frag");
	cube1 = std::make_unique<Cube>(glm::mat4(1.0f), shader.get());
	shader->useShader();


	glfwGetWindowSize(this->window, &width, &height);

	auto projection = glm::perspective(80.0f, width / (float)height, 0.1f, 20.0f);
	auto view = glm::translate(glm::mat4(1), glm::vec3(0, 0, -2));
	auto view_projection = projection * view;

	auto view_projection_location = glGetUniformLocation(shader->programHandle, "VP");
	glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(view_projection));

	FreeImage_Initialise(true);

	texture1 = std::make_unique<Texture>("cgue-quidditch_simulator/Resources/magda.png");
}


void Game::gameLoop() {

	glClearColor(0.99f, 0.36f, 0.43f, 0.3f);
	glViewport(0, 0, width, height);

	bool running = true;
	auto time = glfwGetTime();


	while (running && !glfwWindowShouldClose(window)) {
		auto time_new = glfwGetTime();
		auto time_delta = (float)(time_new - time);
		time = time_new;

		std::cout << "frameTime: " << time * 1000 << "ms, " << 1.0 / time_delta << "FPS" << std::endl;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		update(time_delta);
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}
		
	}
	
	cleanUp();
	glfwTerminate();
	
}

void Game::update(float time_delta) {
	cube1->update(time_delta);
}

void Game::cleanUp() {
	shader.reset(nullptr);
	cube1.reset(nullptr);
	texture1.reset(nullptr);

}

void Game::draw() {
	auto& model = cube1->modelMatrix;
	auto model_location = glGetUniformLocation(shader->programHandle, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	shader->useShader();
	texture1->bind(0);
	auto texture_location = glGetUniformLocation(shader->programHandle, "color_texture");
	glUniform1i(texture_location, 0);

	cube1->draw();

}

//source from https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html 
void Game::processKeyInput(float time_delta) {

	if (!quit) {
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDLK_LEFT:
			
			break;

		case SDLK_RIGHT:

			break;

		case SDLK_UP:

			break;

		case SDLK_DOWN:

			break;

		case SDLK_LSHIFT:

			break;

		case SDLK_SPACE:

			break;

		case SDL_QUIT:
			quit = true;
			break;
		
		}
	}
}
