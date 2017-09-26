#ifndef OGLE_MODEL_HPP
#define OGLE_MODEL_HPP

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ogle/mesh.hpp"
#include "ogle/shader.hpp"
#include "ogle/texture.hpp"

namespace ogle {

class Model
{
public:
	Model(const std::string &filename);

	void Draw(ogle::Shader &shader);
private:
	/* model data */
	std::vector<ogle::Mesh> meshes;
	std::string directory;
	
	void loadModel(std::string &filename);
	void processNode(aiNode *node, const aiScene *scene);
	ogle::Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<ogle::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

}

#endif // OGLE_MODEL_HPP
