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

Object* Model::loadNode(GameManager* gm, aiNode * node, const aiScene * scene)
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
		object->addComponent(mesh);
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

	//Check Textures
	//TODO: Check for other texture types
	//TODO: Check for embedded textures

	//Diffuse Textures
	loadTexture(material, gMaterial, aiTextureType_DIFFUSE, scene);
	
	gMaterial->gm = gm;
	return gMaterial;
}

void Model::loadTexture(aiMaterial * aiMat, Material * gMat, aiTextureType textType, const aiScene* scene)
{
	aiString path;
	TextureTypes gTextType;
	switch (textType) {
	case aiTextureType_DIFFUSE:
		gTextType = DIFFUSE_TEXTURE;
		break;
	default:
		gTextType = DIFFUSE_TEXTURE;
	}
	if (aiMat->GetTexture(textType, 0, &path) == AI_SUCCESS) {
		grTexture* texture = new grTexture();
		cout << "Path:" << path.C_Str() << endl;
		if (const aiTexture* textureData = scene->GetEmbeddedTexture(path.C_Str()))
		{
			cout << "Embedded Texture:" << textureData << endl;
			////Load embedded texture
			int width, height, nrComponents;
			unsigned char* data = this->loadEmbeddedTexture(textureData, &width, &height, &nrComponents);
			texture->loadTexture(data, width, height);
		}
		else
		{
			//Handle absolute texture paths
			int idx = std::string(path.data).rfind("\\");
			std::string filename = std::string(path.data).substr(idx + 1);
			filename = this->directory + "/" + filename;
			texture->loadTexture(filename.c_str());
		}
		gMat->setTexture(texture, gTextType);
	}
}

unsigned char * Model::loadEmbeddedTexture(const aiTexture *textureData, int *width, int *height, int *nrComponents)
{
	unsigned char* data = nullptr;
	int desiredChannels;
	if (textureData->mHeight == 0)
	{
		if (textureData->achFormatHint[0] == 'j')
		{
			cout << "JPEG" << endl;
			desiredChannels = 3;
		}
		else if (textureData->achFormatHint[0] == 'p')
		{
			cout << "PNG" << endl;
			desiredChannels = 4;
		}
		uint8_t* t_data = reinterpret_cast<unsigned char*>(textureData->pcData);
		data = stbi_load_from_memory(t_data, textureData->mWidth, width, height, nrComponents, 4);
	}
	else {
		data = new unsigned char(textureData->mWidth*textureData->mHeight * 4);
		aiTexel* texels = textureData->pcData;
		for (int i = 0; i < textureData->mWidth*textureData->mHeight; i++)
		{
			unsigned char r = texels->r;
			unsigned char g = texels->g;
			unsigned char b = texels->b;
			unsigned char a = texels->a;
			data[4 * i] = r;
			data[4 * i + 1] = g;
			data[4 * i + 2] = b;
			data[4 * i + 3] = a;
		}
		*width = textureData->mWidth;
		*height = textureData->mHeight;
	}
	return data;
}
