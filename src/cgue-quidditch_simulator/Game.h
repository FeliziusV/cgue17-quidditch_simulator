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
	ModelShader modelShader;
	std::unique_ptr<Camera> camera;

	std::unique_ptr<cgue::scene::Cube> cube1;
	std::unique_ptr<cgue::Texture> texture1;

	std::unique_ptr<cgue::scene::Cube> cube2;
	std::unique_ptr<cgue::Texture> texture2;

	std::unique_ptr<cgue::scene::Cube> cube3;
	glm::vec3 cube3loc;
	std::unique_ptr<cgue::Texture> texture3;


	Model nanoSuit;


	std::unique_ptr<PointLight> pointLight;

	int width;
	int height;
	float time_delta;
	glm::mat4 projection;
};





