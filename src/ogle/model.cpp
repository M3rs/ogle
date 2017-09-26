#include "ogle/model.hpp"

#include <iostream>

#include "glm/glm.hpp"

namespace ogle {

Model::Model(const std::string &filename)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = filename.substr(0, filename.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::Draw(ogle::Shader &shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	// process all the nodes meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	
	// then do the same for each of it's children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

ogle::Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<ogle::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ogle::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		} else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// this is always true
	//if (mesh->mMaterialIndex >= 0) {
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<ogle::Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<ogle::Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}
	
	return Mesh(vertices, indices, textures);
}

std::vector<ogle::Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<ogle::Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		ogle::Texture texture { (directory + "/" + str.C_Str()) };
		texture.set_type(typeName);
		textures.push_back(std::move(texture));
		//textures.push_back(texture);
	}

	return textures;
}

}
