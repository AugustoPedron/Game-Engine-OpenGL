#include "ModelLoader.h"

ModelLoader::ModelLoader(ModelDefinition model, GLuint id) {
	this->id = id;
	this->moving = 0;
	this->position = model.position;
	this->positionPhy = model.positionPhy;
	this->path = model.path;
	this->loadModel();
}

void ModelLoader::Draw(ShaderLoader* shader, unsigned int shadowMap) {
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		shader->SetMat4("model", this->position);
		this->meshes[i].Draw(shader, shadowMap);
	}
}

void ModelLoader::loadModel() {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(this->path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));
	this->processNode(scene->mRootNode, scene);
}

void ModelLoader::processNode(aiNode *node, const aiScene *scene) {
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		MeshLoader ML = this->processMesh(mesh, scene);
		this->meshes.push_back(ML);
	}

	for (GLuint i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

MeshLoader ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<structTexture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		/*if (mesh->mVertices[i].x > maxx) maxx = mesh->mVertices[i].x;
		if (mesh->mVertices[i].x < minx) minx = mesh->mVertices[i].x;

		if (mesh->mVertices[i].y > maxy) maxy = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y < miny) miny = mesh->mVertices[i].y;

		if (mesh->mVertices[i].z > maxz) maxz = mesh->mVertices[i].z;
		if (mesh->mVertices[i].z < minz) minz = mesh->mVertices[i].z;*/

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
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<structTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<structTexture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	/*maxx >= minx * -1 ? diffx = maxx : diffx = minx * -1;
	maxy >= miny * -1 ? diffy = maxy : diffy = miny * -1;
	maxz >= minz * -1 ? diffz = maxz : diffz = minz * -1;*/

	return MeshLoader(vertices, indices, textures);
}

std::vector<structTexture> ModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
	std::vector<structTexture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str) {
				textures.push_back(textures_loaded[j]);
				skip = true;

				break;
			}
		}

		if (!skip) {
			structTexture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);

			this->textures_loaded.push_back(texture);
		}
	}

	return textures;
}

GLuint ModelLoader::TextureFromFile(const char *path, std::string directory) {
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char *image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}

void ModelLoader::DeleteModel() {
	std::vector<MeshLoader>::iterator it = this->meshes.begin();
	for (it; it != this->meshes.end(); it++) {
		it->DeleteMesh();
	}
	std::vector<MeshLoader>().swap(this->meshes);
	std::vector<structTexture>().swap(this->textures_loaded);
}

void ModelLoader::updatePosition(glm::vec3 translation) {
	this->positionPhy += translation;
	this->position = glm::translate(this->position, translation);
	this->moving = 1;
	this->lastTranslation = translation;
}