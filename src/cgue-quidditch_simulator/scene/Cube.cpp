#include "Cube.h"

#include <glm/gtc/matrix_transform.hpp>


using namespace cgue;
using namespace cgue::scene;

Cube::Cube()
	: SceneObject(glm::mat4(1.0f)) {


}

Cube::Cube(glm::mat4& matrix, Shader* _shader) 
	: SceneObject(matrix), shader(_shader){

	//Buffer für positionen
	//erstelle einen buffer und schreibe ihn in positionBuffer
	glGenBuffers(1, &positionBuffer);
	//bind positionbuffer als arraybuffer, der irgendwelche attributsdaten speichert
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	//kopiere daten auf den gpu speicher drauf
	//cube vertext count ist eine eigene Variable
	glBufferData(GL_ARRAY_BUFFER, CUBE_VERTEX_COUNT * 3 * sizeof(glm::vec3), new_positions, GL_STATIC_DRAW);
	//Wenn daten hochgeladen sind kann der buffer unbinded werden
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//*******************************************


	//Erstelle indexbuffer für index daten
	glGenBuffers(1, &indexBuffer);
	//arbeite jetzt mit element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUBE_INDEX_COUNT * sizeof(unsigned int), new_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//**********************************************

	//Erstelle normalsBuffer für flächen coloring
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, CUBE_VERTEX_COUNT * 3 * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//*****************************************************

	//Erstelle uvBuffer für Texturen
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, CUBE_VERTEX_COUNT * 3 * sizeof(glm::vec2), uvs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//*****************************************************


	// Erstelle VAO und speichere sie in vao (klassenvariable)
	glGenVertexArrays(1, &vao);
	//bind das vao
	glBindVertexArray(vao);

	//setze vertex-attribute-pointer auf
	//dazu den positionbuffer binden
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	//welche location hat die positionsvariable im shader (shortcut mit layout dingsi siehe day 3 folien)
	auto positionIndex = glGetAttribLocation(shader->programHandle, "position");
	//enable die location und übergebe den abgefragten index
	glEnableVertexAttribArray(positionIndex);
	//teile mit wie die daten zu interpretieren sind
	//zuerst die location, wiviele datenelemente gehören dazu, welcher typ sind die daten, rest standartwerte
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//******************************************************

	//setze attribute pointer für nomals
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	auto normalIndex = glGetAttribLocation(shader->programHandle, "normal");
	glEnableVertexAttribArray(normalIndex);
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//*************************************************************

	//setze attribute-pointer für uv-buffer
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	auto uvIndex = glGetAttribLocation(shader->programHandle, "uv");
	glEnableVertexAttribArray(uvIndex);
	glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//****************************************************


	//bind elementarray buffer um auch den indexbuffer im vao zu haben
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	//jetzt noch unbinden zuerst das vao unbinden, dann die vbos
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Cube::~Cube() {
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &normalsBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vao);
}

void Cube::update(float time_delta) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f) * time_delta, glm::vec3(0, 1, 0));


}

void Cube::draw() {
	//aktiviere das vao
	glBindVertexArray(vao);

	//danach die daten zeichnen
	//parameter: 1: welche art und weise sollen die daten gezeichnet werden, 2: wieviele elemente sollen gezeichent werden
	//3: Welche format liegen die indexdaten vor, 4: bei welcher location soll es starten
	glDrawElements(GL_TRIANGLES, CUBE_INDEX_COUNT, GL_UNSIGNED_INT, 0);

	//unbind das vao um keine nachfolgenden operationen zu beeinflussen
	glBindVertexArray(0);
}





//Vertex positions for Cube
const float Cube::positions[CUBE_VERTEX_COUNT * 3] = {

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,

	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f
};

//beschreibt welche eckpunkte zu dreiecken zusammengefasst werden
const unsigned int Cube::indices[CUBE_INDEX_COUNT] = {
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	0, 1, 4,
	1, 5, 4,

	2, 3, 6,
	3, 7, 6,

	1, 2, 6,
	1, 6, 5,

	0, 4, 7,
	0, 7, 3
};

const unsigned int Cube::new_indices[CUBE_INDEX_COUNT] = {
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	8, 9, 10,
	8, 10, 11,

	12, 13, 14,
	12, 14, 15,

	16, 17, 18,
	16, 18, 19,

	20, 21, 22,
	20, 22, 23

};

//new vertex positions for Cube
const float Cube::new_positions[24 * 3] = {

	//Back
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	//Front
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	//Top
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,

	//Bottom
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	//Right
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,

	//Left
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f

};

//normals of cube for coloring 
const float Cube::normals[24 * 3] = {

	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f

};

//for texturess
const float Cube::uvs[24 * 2] = {
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};