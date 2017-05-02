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


	//cube 1
	//******************************
	shader = std::make_unique<Shader>("Shader/basic.vert", "Shader/basic.frag");
	cube1 = std::make_unique<Cube>(glm::mat4(1.0f), shader.get());
	cube1->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100, 0.05, 100));
	shader->useShader();
	glfwGetWindowSize(this->window, &width, &height);

	glm::vec3 position = glm::vec3(0, -2, 0.001f);
	glm::vec3 lookAt = glm::vec3(0, 0, 1);
	camera = std::make_unique<Camera>(position, lookAt);
	camera->project(width, height, 0.05f, 100.0f);

	auto view_projection_location = glGetUniformLocation(shader->programHandle, "VP");
	glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(camera->view_projection));

	FreeImage_Initialise(true);

	texture1 = std::make_unique<Texture>("cgue-quidditch_simulator/Resources/chessboard.jpg");
}


void Game::gameLoop() {

	glClearColor(0.99f, 0.36f, 0.43f, 0.3f);
	glViewport(0, 0, width, height);

	bool running = true;
	auto time = glfwGetTime();


	while (running && !glfwWindowShouldClose(window)) {
		auto time_new = glfwGetTime();
		time_delta = (float)(time_new - time);
		time = time_new;

		std::cout << "frameTime: " << time * 1000 << "ms, " << 1.0 / time_delta << "FPS" << std::endl;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update(time_delta);
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();

	
	}
	
	cleanUp();
	glfwTerminate();
	
}

void Game::update(float time_delta) {
	//->update(time_delta);
	camera->update(time_delta);

	auto view_projection_location = glGetUniformLocation(shader->programHandle, "VP");
	glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(camera->view_projection));
}

void Game::cleanUp() {
	shader.reset(nullptr);
	cube1.reset(nullptr);
	texture1.reset(nullptr);
	camera.reset(nullptr);

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

