#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
}

Object* Model::loadModel(GameManager* gm, const std::string & fileName)
{
	this->directory = fileName.substr(0, fileName.find_last_of('/'));
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return nullptr;
	}
	
	Object* root = this->loadNode(gm, scene->mRootNode, scene);

	//Root transform
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;
	aiMatrix4x4 rootTransform = scene->mRootNode->mTransformation;
	glm::decompose(AiToGLMMat4(rootTransform), scale, orientation, position, glm::vec3(), glm::vec4());
	root->position = position;
	root->scale = scale;
	root->rotation = glm::eulerAngles(orientation);
	return root;
}

Object* Model::loadNode(GameManager* gm, aiNode * node, const aiScene * scene, Object* parent)
{
	Object* object = new Object();
	//Decompose transformation of the mesh
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;
	aiMatrix4x4 nodeTransform = node->mTransformation;
	glm::decompose(AiToGLMMat4(nodeTransform), scale, orientation,position, glm::vec3(), glm::vec4());
	object->position = position;
	object->scale = scale;
	object->rotation = glm::eulerAngles(orientation);
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		Mesh* mesh = this->loadMesh(gm, scene->mMeshes[node->mMeshes[i]], scene);
		object->add(mesh);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		Object* child = this->loadNode(gm, node->mChildren[i], scene);
		object->add(child);
	}
	return object;
}

Mesh* Model::loadMesh(GameManager* gm, aiMesh * mesh, const aiScene * scene)
{
	//cout << "Loading Mesh : " << mesh->mName.data << endl;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		//Normals
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		//UVs
		if (mesh->mTextureCoords[0])
		{
			vertex.UVs = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		vertices.push_back(vertex);
	}

	for (int i = 0; i <mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	Geometry* geometry = new Geometry(vertices, indices);
	Mesh* grMesh;
	if (mesh->mMaterialIndex >= 0) {
		//cout << "Material Index:" << mesh->mMaterialIndex << endl;
		grMesh = new Mesh(geometry, this->loadMaterial(gm, scene, mesh->mMaterialIndex));
	}
	else
	{
		Material* mat = new Material();
		grMesh = new Mesh(geometry, mat);
	}
	grMesh->name = mesh->mName.data;
	grMesh->gm = gm;
	//cout << "Mesh Name:" << grMesh->name << " - Texture Name:" << grMesh->material->getTexture(DIFFUSE_TEXTURE)->getFilename() << endl;
	grMesh->material->gm = gm;

	return grMesh;
}

void Model::loadMaterials(const aiScene * scene)
{
	int materialCount = scene->mNumMaterials;
	for (size_t i = 0; i < materialCount; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
	}
}

Material* Model::loadMaterial(GameManager* gm, const aiScene * scene, unsigned int materialIndex)
{

	Material* gMaterial = new Material();
	aiMaterial* material = scene->mMaterials[materialIndex];
	aiColor3D diffuseColor, specularColor, ambientColor;
	float shininess;
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	material->Get(AI_MATKEY_SHININESS, shininess);
	
	gMaterial->diffuse = this->aiColorToGlm(diffuseColor);
	gMaterial->specular = this->aiColorToGlm(specularColor);
	gMaterial->ambient = this->aiColorToGlm(ambientColor);
	gMaterial->shininess = shininess;
	if (gMaterial->shininess < 2)
	{
		gMaterial->shininess = 2;
	}
	//Check Textures
	//TODO: Check for other texture types
	if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

			//Handle absolute texture paths
			int idx = std::string(path.data).rfind("\\");
			std::string filename = std::string(path.data).substr(idx + 1);
			filename = this->directory + "/" + filename;
			gMaterial->setTexture(filename.c_str(), DIFFUSE_TEXTURE);
			//cout << "Loading " << filename << endl;

			
		}
	}
	
	gMaterial->gm = gm;
	return gMaterial;
}
