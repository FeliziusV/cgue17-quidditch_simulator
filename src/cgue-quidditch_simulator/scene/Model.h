#pragma once
#include "GL\glew.h"
#include "../shader.h"
#include "Mesh.h"
#include "ModelTexture.h"

#include <iostream>
#include <vector>

using namespace std;

class Model {
public:
	Model(char *path);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
	void draw(cgue::Shader* shader);

private:
	vector<Mesh> meshes;
	string directory;
	vector<Mesh::Texture> textures_loaded;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene * scene);
	vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};