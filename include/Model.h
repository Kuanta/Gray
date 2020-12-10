#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>
#include "grMesh.h"
#include "grTexture.h"
#include "GrAnimation.h"
#include "GrAnimManager.h"
#include "GrSkeleton.h"
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
	static aiMatrix4x4 GLMMat4ToAi(glm::mat4 mat)
	{
		return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	}

	static glm::mat4 AiToGLMMat4(aiMatrix4x4& in_mat)
	{
		glm::mat4 tmp;
		tmp[0][0] = in_mat.a1;
		tmp[1][0] = in_mat.b1;
		tmp[2][0] = in_mat.c1;
		tmp[3][0] = in_mat.d1;

		tmp[0][1] = in_mat.a2;
		tmp[1][1] = in_mat.b2;
		tmp[2][1] = in_mat.c2;
		tmp[3][1] = in_mat.d2;

		tmp[0][2] = in_mat.a3;
		tmp[1][2] = in_mat.b3;
		tmp[2][2] = in_mat.c3;
		tmp[3][2] = in_mat.d3;

		tmp[0][3] = in_mat.a4;
		tmp[1][3] = in_mat.b4;
		tmp[2][3] = in_mat.c4;
		tmp[3][3] = in_mat.d4;
		return tmp;
	}
	Object* loadModel(GameManager* gm, const std::string& fileName);
	Object* loadedModel = nullptr;
	void importAnimations(const std::string& fileName);
	static vector<GrAnimation*> loadAnimations(const aiScene* scene);
	void cleanup();
private:
	string directory;
	Object* root;
	GrSkeleton* skeleton;

	//Methods
	Object* loadNode(GameManager* gm, aiNode *node, const aiScene* scene, bool isRoot = false);
	GrMesh* loadMesh(GameManager* gm, aiNode* node, aiMesh *mesh, const aiScene* scene);
	void loadMaterials(const aiScene* scene);
	Material* loadMaterial(GameManager* gm, const aiScene* scene, unsigned int materialIndex);
	void loadTexture(aiMaterial* aiMat, Material* gMat, aiTextureType textType, const aiScene* scene);
	void loadBones(const aiScene* scene, aiNode* node, aiMesh* aiMesh, Geometry* geometry);

	unsigned char* loadEmbeddedTexture(const aiTexture * textureData, int *width, int *height, int *nrComponents);
	//Assimp To Glm conversions
	glm::vec3 aiColorToGlm(aiColor3D aiColor) {
		return glm::vec3(aiColor.r, aiColor.g, aiColor.b);
	}
};
