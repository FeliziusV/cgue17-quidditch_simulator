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

Game::Game(): modelShader("Shader/model.vert", "Shader/model.frag"), nanoSuit("cgue-quidditch_simulator/Resources/nanosuit/nanosuit.obj") {


}

Game::~Game() {

}

void Game::init(GLFWwindow* window)
{

	this->window = window;
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowTitle(this->window, "CGUE Project");

	//ModelShader modelShader("Shader/model.vert", "Shader/model.frag");

	//Model nanoSuit 
	//**********************************************
	//nanoSuit = std::make_unique<Model>("cgue-quidditch_simulator/Resources/nanosuit/nanosuit.obj");
	//Model nanoSuit("cgue-quidditch_simulator/Resources/nanosuit/nanosuit.obj");


	//pointLight
	//***********************************************
	pointLight = std::make_unique<PointLight>(glm::vec3(4.0f, 6.0f, -5.0f));


	modelShader.use();

	glfwGetWindowSize(this->window, &width, &height);

	glm::vec3 position = glm::vec3(0, 2.0f, 0);
	camera = std::make_unique<Camera>(position);
	projection = glm::perspective(45.0f, width / (float)height, 0.1f, 200.0f);

	FreeImage_Initialise(true);


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
	pointLight->move(time_delta);
}

void Game::cleanUp() {
	
	camera.reset(nullptr);
	pointLight.reset(nullptr);

}

void Game::draw() {
	//load VP Matrix to Shader
	glm::mat4 view_projection = projection * camera->view;
	auto view_projection_location = glGetUniformLocation(modelShader.ID, "VP");
	glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(view_projection));

	//load Light Location to Shader
	glm::vec3 pointLightPosition = pointLight->getPosition();
	//auto pointLightLocation = glGetUniformLocation(shader->programHandle, "pointLightPos");
	//glUniform3f(pointLightLocation, pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);

	//load Camera Location to Shader
	//auto cameraLocation = glGetUniformLocation(shader->programHandle, "cameraPos");
	//glUniform3f(cameraLocation, camera->position.x, camera->position.y, camera->position.z);

	

	//nanosuit with model shader
	//**********************************************
	modelShader.use();
	//load camera
	
	 
	//load Light Location to Shader
	//auto pointLightLocation = glGetUniformLocation(modelShader->programHandle, "pointLightPos");
	//glUniform3f(pointLightLocation, pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);

	//load Camera Location to Shader
	//auto cameraLocation = glGetUniformLocation(modelShader->programHandle, "cameraPos");
	//glUniform3f(cameraLocation, camera->position.x, camera->position.y, camera->position.z);

	auto model_location = glGetUniformLocation(modelShader.ID, "model");
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -20.0f, -100.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	nanoSuit.draw(modelShader);

}

