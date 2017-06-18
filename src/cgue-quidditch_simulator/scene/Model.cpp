#include "Model.h"
#include "ModelTexture.h"

#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include <stb_image.h>

Model::Model(char* path) {
	this->loadModel(path);
}

void Model::draw(Shader* shader) {
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].draw(shader);
	}
}

void Model::loadModel(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//import model and translate it in assimps scene object...
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Error::ASSIMO::" << importer.GetErrorString() << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);	//process node recursive call until all nodes are processed

}

void Model::processNode(aiNode* node, const aiScene* scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {	// for all meshes in node
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	// load meshes
		this->meshes.push_back(this->processMesh(mesh, scene));	// process meshes by using processMesh which
																// returns a Mesh object that we save in meshes
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);	//do all over again for all children node and nodes
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Mesh::Vertex> vertices;
	vector<unsigned int> indices;
	vector<Mesh::Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		//Vertices
		Mesh::Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		//normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//textures
		if (mesh->mTextureCoords[0]) {
			
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	// a mesh contains an array of faces, each face represents a primitive (triangle) 
	// a face contains all vertices we need to draw and define the order too
	// so we iterate over all faces and save vertices of the faces in indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Mesh::Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Mesh::Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<Mesh::Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Mesh::Texture texture;
			/*auto textureObject = std::make_unique<ModelTexture>(str.C_Str(), this->directory);
			texture.id = textureObject->loadTexture();*/
			texture.id = TextureFromFile(str.C_Str(), this->directory, false);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);	// add to loaded textures
		}
		
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
			if (nrComponents == 1) {
				format = GL_RED;
			}
			else if (nrComponents == 3) {
				format = GL_RGB;
			}
			else if (nrComponents == 4) {
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

