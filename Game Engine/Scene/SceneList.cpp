#include "SceneList.h"

SceneComponents SceneList::getSceneComp() {
	return this->SceneComp;
}

std::vector<glm::mat4> SceneList::getMeshesPositions() {
	return this->MeshesPositions;
}

std::vector<glm::mat4> SceneList::getModelsPositions() {
	return this->ModelsPositions;
}

void SceneList::selectScene(GLuint number) {
	switch (number)
	{

	case 0: {

#pragma region Meshes

		MeshStruct mesh;

		mesh.texture_diffuse = "../Game Engine/Resources/assets/container2.png";
		mesh.texture_specular = "../Game Engine/Resources/assets/container2_specular.png";

		std::vector<Vertex> Vertices;
		GLfloat vertices[] = {
			//posizioni           //normale alla faccia //coordinata della texture
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  0.0f, 1.0f,
			
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
			// Left face
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 // Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			// Top face
		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
		};

		for (GLuint i = 0; i < 288;) {
			glm::vec3 vec1(0.0f);
			vec1.x = vertices[i++];
			vec1.y = vertices[i++];
			vec1.z = vertices[i++];

			glm::vec3 vec2(0.0f);
			vec2.x = vertices[i++];
			vec2.y = vertices[i++];
			vec2.z = vertices[i++];

			glm::vec2 vec3(0.0f);
			vec3.x = vertices[i++];
			vec3.y = vertices[i++];

			Vertex v;
			v.Position = vec1;
			v.Normal = vec2;
			v.TexCoords = vec3;

			Vertices.push_back(v);
		}

		mesh.Vertices = Vertices;
		SceneComp.Meshes.push_back(mesh);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		this->MeshesPositions.push_back(model);
#pragma endregion

#pragma region Skybox

		this->SceneComp.Skybox.Vertices = {
			// Positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		this->SceneComp.Skybox.Textures.push_back("../Game Engine/Resources/skybox/right.tga");
		this->SceneComp.Skybox.Textures.push_back("../Game Engine/Resources/skybox/left.tga");
		this->SceneComp.Skybox.Textures.push_back("../Game Engine/Resources/skybox/top.tga");
		this->SceneComp.Skybox.Textures.push_back("../Game Engine/Resources/skybox/bottom.tga");
		this->SceneComp.Skybox.Textures.push_back("../Game Engine/Resources/skybox/back.tga");
		this->SceneComp.Skybox.Textures.push_back("../Game Engine/Resources/skybox/front.tga");

#pragma endregion

#pragma region Shaders

		SceneComp.Shaders.push_back("lighting");
		SceneComp.Shaders.push_back("modelLoading");
		SceneComp.Shaders.push_back("cubeMap");
		SceneComp.Shaders.push_back("shadowShader");

#pragma endregion

#pragma region Models

		SceneComp.Models.push_back("../Game Engine/Resources/models/nanosuit/nanosuit.obj");
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		this->ModelsPositions.push_back(model);

#pragma endregion
	}

	default:
		break;
	}
}