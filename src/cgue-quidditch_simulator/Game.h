#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SDL2\SDL.h>
#include "shader1.h"
#include "scene\Cube.h"
#include "scene\Texture.h"
#include "scene\Camera.h"

using namespace cgue;

class Game {
public:
	Game();

	void Game::init(GLFWwindow* window);
	void Game::gameLoop();
	void Game::update(float time_delta);
	void Game::cleanUp();
	void Game::draw();


	GLFWwindow* window;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Camera> camera;

	std::unique_ptr<cgue::scene::Cube> cube1;
	std::unique_ptr<cgue::Texture> texture1;

	int width;
	int height;
	float time_delta;
};





