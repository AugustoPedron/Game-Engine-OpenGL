#include "pch.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL2/SOIL2.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Mesh/Mesh.h"
#include "Model/Model.h"
#include "Texture/Texture.h"
#include "Fisica/Luci/Spot_Light.h"
#include "Fisica/Luci/Directional_Light.h"
#include "Fisica/Luci/Point_Light.h"

const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//funzioni per la gestione dei comandi
void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallBack(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
//funzioni per inizializzare il vertex array object e il buffer array object
//il VAO specifica come sono memorizzati gli attributi dei vertici nel VBO
void SetupVertexArrayObj(GLuint *VAO);
void SetupVertexBufferObj(GLuint *VBO);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f);

int main() {
	//inizializzazione di opengl
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //metodo più recente
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//creazione di una nuova finestra
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Prototipo", nullptr, nullptr);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	//assegnazione delle funzioni per la gestione dei comandi
	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetCursorPosCallback(window, MouseCallBack);
	//glfwSetScrollCallback(window, ScrollCallBack);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "failed to initialize glew" << std::endl;
		return EXIT_FAILURE;
	}

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<Vertex> Vertices;
	GLfloat vertices[] = {
		//posizioni           //normale alla faccia //coordinata della texture
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,
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

	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};

	std::vector<Vertex> VerticiPiano;
	GLfloat verticiPiano[] = {
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

	for (GLuint i = 0; i < 48;) {
		glm::vec3 vec1(0.0f);
		vec1.x = verticiPiano[i++];
		vec1.y = verticiPiano[i++];
		vec1.z = verticiPiano[i++];

		glm::vec3 vec2(0.0f);
		vec2.x = verticiPiano[i++];
		vec2.y = verticiPiano[i++];
		vec2.z = verticiPiano[i++];

		glm::vec2 vec3(0.0f);
		vec3.x = verticiPiano[i++];
		vec3.y = verticiPiano[i++];

		Vertex v;
		v.Position = vec1;
		v.Normal = vec2;
		v.TexCoords = vec3;

		VerticiPiano.push_back(v);
	}
	
	//creazione dello shader per disegnare un cubo
	Shader lightingShader("GLSL/lighting.vs", "GLSL/lighting.frag");
	//creazione della mesh del cubo
	Mesh cubo(Vertices, "Resources/assets/container2.png", "Resources/assets/container2_specular.png");
	Mesh piano(VerticiPiano, "Resources/assets/container2.png", "Resources/assets/container2_specular.png");
	glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	
	Shader shader("GLSL/modelLoading.vs", "GLSL/modelLoading.frag");
	Model ourModel("Resources/models/nanosuit/nanosuit.obj");

	//creazione dello shader per disegnare lo sfondo della scena
	Shader skyboxShader("GLSL/cubeMap.vs", "GLSL/cubeMap.frag");

	GLfloat skyboxVertices[] = {
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

	GLuint skyboxVAO, skyboxVBO;
	SetupVertexArrayObj(&skyboxVAO);
	SetupVertexBufferObj(&skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);

	//caricamento delle texture dei lati del cubo per la scena NON FUNZIONANTE ANCORA
	std::vector<const GLchar*> faces;
	faces.push_back("Resources/skybox/right.tga");
	faces.push_back("Resources/skybox/left.tga");
	faces.push_back("Resources/skybox/top.tga");
	faces.push_back("Resources/skybox/bottom.tga");
	faces.push_back("Resources/skybox/back.tga");
	faces.push_back("Resources/skybox/front.tga");
	GLuint skyboxTexture = TextureLoader::LoadSkyboxMap(faces);

	GLint frames = 0;
	GLfloat lastTime = glfwGetTime();
	//loop dove vengono disegnati gli oggetti e gestiti i comandi
	while (!glfwWindowShouldClose(window)) {

		glm::mat4 view = camera.GetViewMatrix();

		//delta time rappresenta la durata dell'ultimo frame, moltiplicando ad esempio la velcità con cui si sposta un oggetto nel tempo per il delta time, si rende il movimento indipendente dal tempo impiegato
		//per la creazione del frame
		frames++;
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		if (currentFrame - lastTime >= 1.0) {
			std::cout << 1000.0/(GLfloat)frames << std::endl;
			frames = 0;
			lastTime += 1.0;
		}
		lastFrame = currentFrame;
		

		glfwPollEvents();
		//applicazione dei movimenti
		DoMovement();

		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//si specifica quale shader program viene usato
		shader.use();
		//viene definito quale tipo di illuminazione applicare al modello. in questo caso gli si da una luce di tipo spot
		DirectionalLightSetter::SetDirectionalLight(shader);
		shader.SetVec3("viewPos", camera.getPosition().x , camera.getPosition().y,  camera.getPosition().z);
		//glGetUniformLocation prende la posizione nel programma dello shader corrispondente al campo specificato tra le virgolette
		//glUniformMatrix4fv usa la posizione ritornata dalla funzione precedente per passare i valori. in questo caso si passa una matrice di 4 valori float.
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", view);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader.SetMat4("model", model);
		//si usa il programma di shader per disegnare a la mesh
		ourModel.Draw(shader);

		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene

		//cubo.DrawShadows(shadowShader, lightPos, lightProjection);
		//glm::mat4 model;
		lightingShader.use();
		lightingShader.SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		lightingShader.SetMat4("view", view);
		lightingShader.SetMat4("projection", projection);
		DirectionalLightSetter::SetDirectionalLight(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(50.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.53f, 0.0f));
		lightingShader.SetMat4("model", model);

		piano.DrawNoIndices(lightingShader, model);

		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		lightingShader.SetMat4("model", model);

		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		//lightingShader.SetMat4("lightSpaceMatrix",lightSpaceMatrix);

		cubo.DrawNoIndices(lightingShader, model);

		glDepthFunc(GL_LEQUAL);

		skyboxShader.use();

		view = glm::mat4(glm::mat3(view));
		skyboxShader.Set1i("skybox", 0);
		skyboxShader.SetMat4("view", view);
		skyboxShader.SetMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);

	}

	/*glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);*/

	glfwTerminate();
	return EXIT_SUCCESS;
}

void DoMovement() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.processKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.processKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.processKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.processKeyboard(RIGHT, deltaTime);
	}
}
//gestisce solo il comando esc per chiudere la finestra
void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (GLFW_PRESS == action) {
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action) {
			keys[key] = false;
		}
	}
}
//muove la camera con il movimento del mouse
void MouseCallBack(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

void ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset) {
	camera.processMouseScroll(yOffset);
}

void SetupVertexArrayObj(GLuint *VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
}

void SetupVertexBufferObj(GLuint *VBO) {
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
}