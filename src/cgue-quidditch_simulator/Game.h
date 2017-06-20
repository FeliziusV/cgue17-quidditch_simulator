#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SDL2\SDL.h>
#include "scene\ModelShader.h"
#include "scene\Cube.h"
#include "scene\Texture.h"
#include "scene\Camera.h"
#include "scene\PointLight.h"
#include "scene\Model.h"

using namespace cgue;

class Game {
public:
	Game();
	~Game();

	void Game::init(GLFWwindow* window);
	void Game::gameLoop();
	void Game::update(float time_delta);
	void Game::cleanUp();
	void Game::draw();


	GLFWwindow* window;
	std::unique_ptr<ModelShader> modelShader;
	std::unique_ptr<ModelShader> skyboxShader;

	std::unique_ptr<Camera> camera;


	std::unique_ptr<Model> nanoSuit;
	//unsigned int loadCubemap(vector<std::string> faces);

	std::unique_ptr<PointLight> pointLight;

	int width;
	int height;
	float time_delta;
	glm::mat4 projection;
};





