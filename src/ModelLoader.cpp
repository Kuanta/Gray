#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
	
}


ModelLoader::~ModelLoader()
{
}

Object* ModelLoader::loadModel(const std::string & fileName)
{
	Object *root = new Object();
	GrSkeleton *skeleton = new GrSkeleton();
	// Initialize skeleton


	this->directory = fileName.substr(0, fileName.find_last_of('/'));
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return nullptr;
	}
	
	//Load bones first
	for(int i=0;i<scene->mNumMeshes;i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
		{
			aiBone *ai_bone = scene->mMeshes[i]->mBones[j];
			glm::mat4 boneMatrix = glm::transpose(ModelLoader::AiToGLMMat4(ai_bone->mOffsetMatrix));
			GrBone *bone = new GrBone(skeleton, boneMatrix, skeleton->globalInverseTransform, ai_bone->mName.data);
			skeleton->addBone(bone->name, bone);
		}
	}
	root = this->loadNode(scene->mRootNode, scene, skeleton, true);
	root->addComponent(skeleton); //Skeleton must be added first

	GrAnimManager* animMan = new GrAnimManager();
	root->addComponent(animMan);
	vector<GrAnimation*> anims = this->loadAnimations(scene);
	animMan->addAnimations(anims);

	//Root transform
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;
	aiMatrix4x4 rootTransform = scene->mRootNode->mTransformation;
	glm::mat4 globalInverseTransform = AiToGLMMat4(rootTransform);

	skeleton->globalInverseTransform = glm::inverse(globalInverseTransform);
	glm::vec3 unitVec3;
	glm::vec4 unitVec4;
	glm::decompose(AiToGLMMat4(rootTransform), scale, orientation, position, unitVec3, unitVec4);
	root->setPosition(position);
	root->setRotation(glm::eulerAngles(orientation));
	root->setScale(scale);



	//Set Bone Parents
	/*
		Some models have a bone hierarchy that is outside of the mesh hierarchy. For example:
		-Spine
			-Head
			-Legs
		-Node
			-Mesh
				-Spine
				-Head
				-Legs
		In the above example, Head and Legs are children of Spine but thats not implied on the Node>Mesh hierarchy.
		To solve this, loop through each bone and find the node, object that has the same name for it. Than check the parent of the found node 
		and search for the bone that has the same name with that parent node. This bone will be the parent node.

	*/
	map<string, GrBone*>::iterator iter;
	for (iter = skeleton->bones.begin(); iter != skeleton->bones.end(); iter++)
	{
		GrBone* bone = iter->second;
		Object* node = root->getChildWithByName(bone->name);
		if (node != nullptr && node->parent != nullptr)
		{
			bone->parentBone = skeleton->getBoneByName(node->parent->name);
			if (bone->parentBone != nullptr)
			{
				bone->parentBone->children.push_back(bone);
			}
		}
	}
	
	//Calculate bind matrices after setting all the parenthoods
	for (iter = skeleton->bones.begin(); iter != skeleton->bones.end(); iter++)
	{
		GrBone* bone = iter->second;
		bone->calculateInverseBindMatrix();
		bone->setToBindPosition();
	}
	return root;
}

void ModelLoader::importAnimations(Object* root, const std::string& fileName, const char* animationName)
{
	if (root == nullptr)
	{
		cout<<"Null Root"<<endl;
	}
	this->directory = fileName.substr(0, fileName.find_last_of('/'));
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES, 0);
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	if (scene == nullptr) // if is Not Zero
	{
		cout << "Couldn't load the animation file" << endl;
		return;
	}
	vector<GrAnimation*> anims = this->loadAnimations(scene);
	anims.at(0)->name = animationName;
	GrAnimManager* animMan = (GrAnimManager*) root->getComponentByType(ComponentType::ANIMATION_MANAGER);
	animMan->addAnimations(anims);

}

vector<GrAnimation*> ModelLoader::loadAnimations(const aiScene* scene)
{
	vector<GrAnimation*> animations;
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];
		cout << "Loading " << anim->mName.data << endl;

		GrAnimation* grAnim = new GrAnimation(anim->mName.data, anim->mDuration, anim->mTicksPerSecond);
		for (int j = 0; j < anim->mNumChannels; j++)
		{
			aiNodeAnim* animNode = anim->mChannels[j];
			GrAnimationNode* node = new GrAnimationNode(animNode);
			grAnim->animNodes.insert(std::pair<string, GrAnimationNode*>(node->name, node));
		}
		animations.push_back(grAnim);
	}
	return animations;
}

Object* ModelLoader::loadNode(aiNode * node, const aiScene * scene, GrSkeleton* skeleton, bool isRoot)
{
	Object* object = new Object();
	object->name = node->mName.data;
	//Decompose transformation of the mesh
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;
	aiMatrix4x4 nodeTransform = node->mTransformation;

	glm::vec3 unitVec3;
	glm::vec4 unitVec4;
	glm::decompose(glm::transpose(AiToGLMMat4(nodeTransform)), scale, orientation, position, unitVec3, unitVec4);
	object->setPosition(position);
	object->setRotation(glm::eulerAngles(orientation));
	object->setScale(scale);
	if (node->mNumMeshes > 0)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			GrMesh* mesh = this->loadMesh(node, scene->mMeshes[node->mMeshes[i]], scene);
			//Load Bones
			loadBones(scene, node, scene->mMeshes[node->mMeshes[i]], skeleton, mesh->geometry);
			mesh->geometry->initBuffers(); //Init buffers after loading bones
			object->addComponent(mesh);
		}
	}
	
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		Object* child = this->loadNode(node->mChildren[i], scene, skeleton, false);
		object->add(child);
	}
	
	return object;
}

GrMesh* ModelLoader::loadMesh(aiNode* node, aiMesh * mesh, const aiScene * scene)
{

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		//Normals
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

		//UVs
		if (mesh->mTextureCoords[0])
		{
			vertex.UVs = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		//Initialize boneIDs (Will set correct values while loading bones)
		for (int j = 0; j < Geometry::NUM_BONES_PER_VERTEX; j++)
		{
			vertex.IDs[j] = -1;
			vertex.Weights[j] = 0;
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

	
	Geometry* geometry = new Geometry(vertices, indices, false);
	GrMesh* grMesh;
	if (mesh->mMaterialIndex >= 0) {
		grMesh = new GrMesh(geometry, this->loadMaterial(scene, mesh->mMaterialIndex), mesh->mName.data);
	}
	else
	{
		Material* mat = new Material();
		grMesh = new GrMesh(geometry, mat, mesh->mName.data);
	}

	return grMesh;
}

void ModelLoader::loadMaterials(const aiScene * scene)
{
	int materialCount = scene->mNumMaterials;
	for (size_t i = 0; i < materialCount; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
	}
}

Material* ModelLoader::loadMaterial(const aiScene * scene, unsigned int materialIndex)
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
	gMaterial->rougness = 0.2f;
	gMaterial->shininess = shininess;
	gMaterial->metalness = false;

	//Check Textures
	//TODO: Check for other texture types
	//TODO: Check for embedded textures

	//Diffuse Textures
	loadTexture(material, gMaterial, aiTextureType_DIFFUSE, scene);
	loadTexture(material, gMaterial, aiTextureType_NORMALS, scene);
	loadTexture(material, gMaterial, aiTextureType_SHININESS, scene);
	
	return gMaterial;
}

void ModelLoader::loadTexture(aiMaterial * aiMat, Material * gMat, aiTextureType textType, const aiScene* scene)
{

	aiString path;
	TextureTypes gTextType;
	switch (textType) {
	case aiTextureType_DIFFUSE:
		gTextType = DIFFUSE_TEXTURE;
		break;
	case aiTextureType_HEIGHT:  //OBJ has height maps for normal maps
	case aiTextureType_NORMALS:
		gTextType = NORMAL_TEXTURE;
		break;
	case aiTextureType_SHININESS:
		gTextType = ROUGHNESS_TEXTURE;
		break;
	default:
		gTextType = DIFFUSE_TEXTURE;
		break;
	}
	if (aiMat->GetTexture(textType, 0, &path) == AI_SUCCESS) {
		grTexture* texture = new grTexture();
		if (const aiTexture* textureData = scene->GetEmbeddedTexture(path.C_Str()))
		{
			//Load embedded texture
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

void ModelLoader::loadBones(const aiScene *scene, aiNode *node, aiMesh *aiMesh, GrSkeleton *skeleton, Geometry *geometry)
{
	for (int i = 0; i < aiMesh->mNumBones; i++)
	{
		aiBone* ai_bone = aiMesh->mBones[i];
		string boneName = ai_bone->mName.data;
		GrBone* grBone = skeleton->getBoneByName(boneName);
		int boneIndex = skeleton->getBoneIndex(boneName);
		if(!boneName.compare("mixamorig:Hips"))
		{
			cout<<"At hips"<<endl;
		}
		for (int j = 0; j < ai_bone->mNumWeights; j++)
		{
			aiVertexWeight weight = ai_bone->mWeights[j];
			for (int b = 0; b < 4; b++)
			{

				if (geometry->vertices.at(weight.mVertexId).IDs[b] == -1)
				{
					geometry->vertices.at(weight.mVertexId).IDs[b] = boneIndex;
					//TODO: Why multiplying the weight works? (Also try multiplying with a very large number)
					geometry->vertices.at(weight.mVertexId).Weights[b] = weight.mWeight;
					break;
				}
			}
		}
	}
}

unsigned char * ModelLoader::loadEmbeddedTexture(const aiTexture *textureData, int *width, int *height, int *nrComponents)
{
	unsigned char* data = nullptr;
	int desiredChannels;
	if (textureData->mHeight == 0)
	{
		if (textureData->achFormatHint[0] == 'j') //jpeg
		{
			desiredChannels = 3;
		}
		else if (textureData->achFormatHint[0] == 'p') //png
		{
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
