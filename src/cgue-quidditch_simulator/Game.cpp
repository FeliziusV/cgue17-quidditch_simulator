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

Game::~Game() {

}

void Game::init(GLFWwindow* window)
{

	this->window = window;
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowTitle(this->window, "CGUE Project");

	shader = std::make_unique<Shader>("Shader/basic.vert", "Shader/basic.frag");

	//cube 1
	//*********************************************
	cube1 = std::make_unique<Cube>(glm::mat4(1.0f), shader.get());
	cube1->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(300, 0.05, 300));
	//cube 2
	//**********************************************
	cube2 = std::make_unique<Cube>(glm::mat4(1.0f), shader.get());
	cube2->modelMatrix = glm::translate(cube2->modelMatrix, glm::vec3(0, 4, -6));
	//cube2->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4, 4, 0.1));
	//cube 3
	//**********************************************
	cube3 = std::make_unique<Cube>(glm::mat4(1.0f), shader.get());
	auto cube3model = cube3->modelMatrix;
	cube3->modelMatrix = glm::translate(cube3->modelMatrix, glm::vec3(0, 6.0f, -0.8f));
	cube3->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3, 0.1, 6));
	cube3->modelMatrix = glm::rotate(cube3->modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	cube3loc = glm::vec3(0, 6.0f, -0.8f);

	//pointLight
	//***********************************************
	pointLight = std::make_unique<PointLight>(glm::vec3(6.0f, 6.0f, -5.0f));


	shader->useShader();
	glfwGetWindowSize(this->window, &width, &height);

	glm::vec3 position = glm::vec3(0, 2.0f, 0);
	camera = std::make_unique<Camera>(position);
	projection = glm::perspective(45.0f, width / (float)height, 0.1f, 200.0f);

	FreeImage_Initialise(true);

	texture1 = std::make_unique<Texture>("cgue-quidditch_simulator/Resources/chessboard.jpg");
	texture2 = std::make_unique<Texture>("cgue-quidditch_simulator/Resources/ring.png");
	texture3 = std::make_unique<Texture>("cgue-quidditch_simulator/Resources/player.png");
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

		//std::cout << "frameTime: " << time * 1000 << "ms, " << 1.0 / time_delta << "FPS" << std::endl;
		
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

	camera->update(time_delta);
	cube2->update(time_delta);
	//pointLight->move(time_delta);
	//std::cout << "pointLight Pos: " << pointLight->pos.x << std::endl;
	//cube3loc = cube3loc - camera->position;
	//cube3->modelMatrix = glm::translate(cube3->modelMatrix, camera->position);
}

void Game::cleanUp() {
	shader.reset(nullptr);
	cube1.reset(nullptr);
	texture1.reset(nullptr);
	cube2.reset(nullptr);
	texture2.reset(nullptr);
	cube3.reset(nullptr);
	texture3.reset(nullptr);
	camera.reset(nullptr);
	pointLight.reset(nullptr);

}

void Game::draw() {
	//load VP Matrix to Shader
	glm::mat4 view_projection = projection * camera->view;
	auto view_projection_location = glGetUniformLocation(shader->programHandle, "VP");
	glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(view_projection));

	//load Light Location to Shader
	glm::vec3 pointLightPosition = pointLight->getPosition();
	auto pointLightLocation = glGetUniformLocation(shader->programHandle, "pointLightPos");
	glUniformMatrix4fv(pointLightLocation, 1, GL_FALSE, glm::value_ptr(pointLightPosition));


	//cube 1
	//**********************************************
	auto& model1 = cube1->modelMatrix;
	auto model_location1 = glGetUniformLocation(shader->programHandle, "model");
	glUniformMatrix4fv(model_location1, 1, GL_FALSE, glm::value_ptr(model1));
	shader->useShader();
	texture1->bind(0);
	auto texture_location1 = glGetUniformLocation(shader->programHandle, "color_texture");
	glUniform1i(texture_location1, 0);
	cube1->draw();
	//cube 2
	//**********************************************
	auto& model2 = cube2->modelMatrix;
	auto model_location2 = glGetUniformLocation(shader->programHandle, "model");
	glUniformMatrix4fv(model_location2, 1, GL_FALSE, glm::value_ptr(model2));
	shader->useShader();
	texture2->bind(1);
	auto texture_location2 = glGetUniformLocation(shader->programHandle, "color_texture");
	glUniform1i(texture_location2, 1);
	cube2->draw();
	//cube 3
	//**********************************************
	auto& model3 = cube3->modelMatrix;
	auto model_location3 = glGetUniformLocation(shader->programHandle, "model");
	glUniformMatrix4fv(model_location3, 1, GL_FALSE, glm::value_ptr(model3));
	shader->useShader();
	texture3->bind(2);
	auto texture_location3 = glGetUniformLocation(shader->programHandle, "color_texture");
	glUniform1i(texture_location3, 2);
	cube3->draw();



}

