#include "Model.h"
#include "ModelTexture.h"

#include <glm\gtc\type_ptr.hpp>
#include <memory>


Model::Model(GLchar* path) {
	this->loadModel(path);
}

void Model::draw(Shader* shader) {
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].draw(shader);
	}
}

void Model::loadModel(string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Error::ASSIMO::" << import.GetErrorString() << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene) {

	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Mesh::Vertex> vertices;
	vector<GLuint> indices;
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
	//indices
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
		Mesh::Texture texture;
		auto textureObject = std::make_unique<ModelTexture>(str.C_Str(), this->directory);
		texture.id = textureObject->loadTexture();
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}