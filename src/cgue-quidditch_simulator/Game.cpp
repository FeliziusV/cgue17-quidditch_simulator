#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <FreeImage\FreeImage.h>
#include <stb_image.h>

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

	//CubeMap
	//***********************************************
	vector<std::string> faces{
		"cgue-quidditch_simulator/Resources/SkyBox/right.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/left.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/top.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/bottom.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/back.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/front.jpg"
	};
	float cubemapTexture = loadCubemap(faces);
	
	//Shaders
	modelShader = std::make_unique<ModelShader>("Shader/model.vert", "Shader/model.frag");
	skyboxShader = std::make_unique<ModelShader>("Shader/skybox.vert", "Shader/skybox.frag");
	skyboxShader->setInt("skybox", 0);

	//Model nanoSuit 
	//**********************************************
	char* path = "cgue-quidditch_simulator/Resources/nanosuit/nanosuit.obj";
	nanoSuit = std::make_unique<Model>(path);

	//pointLight
	//***********************************************
	pointLight = std::make_unique<PointLight>(glm::vec3(4.0f, 6.0f, -5.0f));

	// camera
	//***********************************************
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
	//______________________________________________
	// Models
	nanoSuit.reset(nullptr);
	modelShader.reset(nullptr);
	skyboxShader.reset(nullptr);

	//______________________________________________
	// camera Lights
	camera.reset(nullptr);
	pointLight.reset(nullptr);

	//______________________________________________
	// Buffers
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVAO);

}

void Game::draw() {

	// cube
	/*glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	*/


	//load VP Matrix to Shader
	glm::mat4 view_projection = projection * camera->view;
	auto view_projection_location = glGetUniformLocation(Game::modelShader->ID, "VP");
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
	modelShader->use();


	//load Light Location to Shader
	//auto pointLightLocation = glGetUniformLocation(modelShader->programHandle, "pointLightPos");
	//glUniform3f(pointLightLocation, pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);

	//load Camera Location to Shader
	//auto cameraLocation = glGetUniformLocation(modelShader->programHandle, "cameraPos");
	//glUniform3f(cameraLocation, camera->position.x, camera->position.y, camera->position.z);

	auto model_location = glGetUniformLocation(modelShader->ID, "model");
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -20.0f, -100.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	nanoSuit->draw(*modelShader);

	//______________________________________________
	// draw skybox
	drawSkybox();
}



unsigned int Game::loadCubemap (vector<std::string> faces) {
	// cube VAO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// load cubemapTexture
	// loads a cubemap texture from 6 individual texture faces
	// order:
	// -X (left), +X (right), +Y (top), -Y (bottom), +Z (front), -Z (back)
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
			stbi_image_free(data);
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	return textureID;
}

void Game::drawSkybox() {
	//draw skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader->use();
	glm::mat4 view = glm::mat4(glm::mat3(camera->view));
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	// draw skybox Cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

const float Game::skyboxVertices[108] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

const float Game::cubeVertices[180] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
