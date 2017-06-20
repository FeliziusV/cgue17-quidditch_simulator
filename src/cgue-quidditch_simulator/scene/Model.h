#pragma once
#include "GL\glew.h"
#include "ModelShader.h"
#include "Mesh.h"
#include "ModelTexture.h"

#include <iostream>
#include <vector>

using namespace std;

class Model {
public:
	vector<Mesh::Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	// Functions
	Model(char *path);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
	void draw(ModelShader shader);

private:

	void loadModel(string const &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene * scene);
	vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};