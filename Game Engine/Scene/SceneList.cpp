#include "SceneList.h"

SceneComponents SceneList::getSceneComp() {
	return this->SceneComp;
}

/*std::vector<glm::mat4> SceneList::getModelsPositions() {
	return this->ModelsPositions;
}*/

void SceneList::selectScene(GLuint number) {
	switch (number)
	{

	case 0: {

#pragma region Meshes

		/*MeshDefinition MeD;

		MeD.mesh.texture_diffuse = "../Game Engine/Resources/assets/container2.png";
		MeD.mesh.texture_specular = "../Game Engine/Resources/assets/container2_specular.png";

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

		MeD.mesh.Vertices = Vertices;
		MeD.position = glm::mat4(1.0f);
		MeD.position = glm::translate(MeD.position, glm::vec3(0.0f, -0.4f, 3.0f));
		MeD.position = glm::scale(MeD.position, glm::vec3(2.0f));

		SceneComp.Meshes.push_back(MeD);



		//this->MeshesPositions.push_back(model);

		GLfloat pianoVertices[] = {
		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
		};

		Vertices.clear();

		for (GLuint i = 0; i < 48;) {
			glm::vec3 vec1(0.0f);
			vec1.x = pianoVertices[i++];
			vec1.y = pianoVertices[i++];
			vec1.z = pianoVertices[i++];

			glm::vec3 vec2(0.0f);
			vec2.x = pianoVertices[i++];
			vec2.y = pianoVertices[i++];
			vec2.z = pianoVertices[i++];

			glm::vec2 vec3(0.0f);
			vec3.x = pianoVertices[i++];
			vec3.y = pianoVertices[i++];

			Vertex v;
			v.Position = vec1;
			v.Normal = vec2;
			v.TexCoords = vec3;

			Vertices.push_back(v);
		}

		MeD.mesh.Vertices = Vertices;

		MeD.position = glm::mat4(1.0f);
		MeD.position = glm::scale(MeD.position, glm::vec3(20.0f));
		MeD.position = glm::translate(MeD.position, glm::vec3(0.0f, -0.59f, 0.0f));
		SceneComp.Meshes.push_back(MeD);

		//this->MeshesPositions.push_back(model);*/
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
		SceneComp.Shaders.push_back("shadowShaderDebug");

#pragma endregion

#pragma region Models

		ModelDefinition MD;

		/*SceneComp.Models.push_back("../Game Engine/Resources/models/prova2.obj");
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		this->ModelsPositions.push_back(model);

		SceneComp.Models.push_back("../Game Engine/Resources/models/castello.obj");
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, 20.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		this->ModelsPositions.push_back(model);*/

		/*MD.path = "../Game Engine/Resources/models/nanosuit/nanosuit.obj";
		MD.position = glm::mat4(1.0f);
		MD.position = glm::translate(MD.position, glm::vec3(0.0f, -1.75f, 0.0f));
		MD.position = glm::scale(MD.position, glm::vec3(0.2f, 0.2f, 0.2f));
		SceneComp.Models.push_back(MD);*/

		MD.pathModel = "../Game Engine/Resources/models/sfera.obj";
		MD.pathBoundingBox = "../Game Engine/Resources/models/sferaBound2.obj";
		MD.position = glm::mat4(1.0f);
		MD.positionPhy = glm::vec3(2.0f, 0.0f, 2.0f);
		//MD.position = glm::rotate(MD.position, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		MD.position = glm::translate(MD.position, MD.positionPhy);
		//MD.position = glm::rotate(MD.position, 20.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//MD.position = glm::rotate(MD.position, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//MD.position = glm::scale(MD.position, glm::vec3(0.2f, 0.2f, 0.2f));
		SceneComp.Models.push_back(MD);

		MD.pathModel = "../Game Engine/Resources/models/cubo.obj";
		MD.pathBoundingBox = "../Game Engine/Resources/models/cubo.obj";
		MD.position = glm::mat4(1.0f);
		MD.positionPhy = glm::vec3(2.0f, -0.7f, -2.0f);
		MD.position = glm::translate(MD.position, MD.positionPhy);	
		//MD.position = glm::rotate(MD.position, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MD.angle = 0.0f;

		glm::vec3 p(1.0, -1.0, 1.0);
		glm::vec3 c(2.0, 0.0, -2.0);
		glm::vec3 sum = p + c;
		p = glm::rotateY(p, glm::radians(45.0f));
		//MD.position = glm::scale(MD.position, glm::vec3(0.2f, 0.2f, 0.2f));
		SceneComp.Models.push_back(MD);


#pragma endregion
	}

	default:
		break;
	}
}