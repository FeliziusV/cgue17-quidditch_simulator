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

	
	//______________________________________________
	//Shaders
	modelShader = std::make_unique<ModelShader>("Shader/model.vert", "Shader/model.frag");
	skyboxShader = std::make_unique<ModelShader>("Shader/skybox.vert", "Shader/skybox.frag");
	cubeMapShader = std::make_unique<ModelShader>("Shader/cubeMap.vert", "Shader/cubeMap.frag");


	//______________________________________________
	// CubeMap
	cubeTexture = loadTexture("cgue-quidditch_simulator/Resources/chessboard.jpg");
	
	//______________________________________________
	// skybox
	vector<std::string> faces{
		"cgue-quidditch_simulator/Resources/SkyBox/right.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/left.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/top.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/bottom.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/back.jpg",
		"cgue-quidditch_simulator/Resources/SkyBox/front.jpg"
	};
	cubemapTexture = loadCubemap(faces);

	//______________________________________________
	// Shader configuration
	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);

	cubeMapShader->use();
	cubeMapShader->setInt("texture1", 0);

	//______________________________________________
	//Model nanoSuit 
	modelShader->use();
	char* path = "cgue-quidditch_simulator/Resources/nanosuit/nanosuit.obj";
	nanoSuit = std::make_unique<Model>(path);

	//______________________________________________
	//pointLight
	pointLight = std::make_unique<PointLight>(glm::vec3(4.0f, 6.0f, -5.0f));

	//______________________________________________
	// camera
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
	//______________________________________________
	// build VP matrix
	glm::mat4 view_projection = projection * camera->view;

	//______________________________________________
	// build model matrix
	glm::mat4 model;

	//______________________________________________
	// load Light Location
	glm::vec3 pointLightPosition = pointLight->getPosition();

	//______________________________________________
	// load camera Location
	glm::vec3 cameraPos;
	cameraPos = glm::vec3(camera->position.x, camera->position.y, camera->position.z);

	//______________________________________________
	//nanosuit with model shader
	modelShader->use();
	//load Camera Location to Shader
	//auto cameraLocation = glGetUniformLocation(modelShader->programHandle, "cameraPos");
	//glUniform3f(cameraLocation, camera->position.x, camera->position.y, camera->position.z);
	
	//load light location to shader
	//auto pointLightLocation = glGetUniformLocation(shader->programHandle, "pointLightPos");
	//glUniform3f(pointLightLocation, pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -20.0f, -100.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	modelShader->setMat4("VP", view_projection);
	modelShader->setMat4("model", model);
	nanoSuit->draw(*modelShader);


	//______________________________________________
	// draw box
	cubeMapShader->use();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -10.0f, -40.0f));
	cubeMapShader->setMat4("model", model);
	cubeMapShader->setMat4("VP", view_projection);
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	
	//______________________________________________
	// draw skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader->use();
	skyboxShader->setMat4("view", glm::mat4(glm::mat3(camera->view)));
	skyboxShader->setMat4("projection", projection);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	
}



unsigned int Game::loadCubemap (vector<std::string> faces) {
	
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
			system("PAUSE");
			exit(EXIT_FAILURE); 
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

GLuint Game::loadTexture(char const *path) {

	//______________________________________________
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

	//______________________________________________
	//skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char * data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1) {
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGB;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else {
		cout << "Texture failed to load at path: " << path << endl;
		system("PAUSE");
		exit(EXIT_FAILURE); 
		stbi_image_free(data);
	}

	return textureID;
}

void Game::drawSkybox() {

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
