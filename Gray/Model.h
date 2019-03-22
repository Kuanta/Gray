#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "grTexture.h"
#include "Object.h"
#include "Geometry.h"
#include "Material.h"
#include "GameManager.h"

class Model
{
public:
	Model();
	~Model();

	//Methods
	Object* loadModel(GameManager* gm, const std::string& fileName);
	void cleanup();
private:
	string directory;

	//Methods
	Object* loadNode(GameManager* gm, aiNode *node, const aiScene* scene, Object* parent = nullptr);
	Mesh* loadMesh(GameManager* gm, aiMesh *mesh, const aiScene* scene);
	void loadMaterials(const aiScene* scene);
	Material* loadMaterial(GameManager* gm, const aiScene* scene, unsigned int materialIndex);

	//Assimp To Glm conversions
	glm::vec3 aiColorToGlm(aiColor3D aiColor) {
		return glm::vec3(aiColor.r, aiColor.g, aiColor.b);
	}
};

