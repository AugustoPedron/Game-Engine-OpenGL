#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Scene/Scene.h"
#include "Collision Detection/Octree/Octree.h"
#include "Fisica/Fisica.h"

const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//funzioni per la gestione dei comandi
void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mode);
//void ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallBack(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

GLuint sceneNumber = 0;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
Camera camera;

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

	
	OctreeBounds bounds;
	bounds.position = glm::vec3(0.0f, -1.75f, 0.0f);
	bounds.size = 200.0f;
	std::shared_ptr<Octree> _Octree(new Octree(NULL, bounds, 20, 0));

	camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));

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

	//glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Scene scene(camera);
	std::shared_ptr<Scene> scene(new Scene(camera));
	scene->loadScene(sceneNumber,_Octree);
	//scene.loadScene(sceneNumber);

	GLint frames = 0;
	GLfloat lastTime = glfwGetTime();
	//loop dove vengono disegnati gli oggetti e gestiti i comandi

	while (!glfwWindowShouldClose(window)) {
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

		//scene.drawScene();
		Fisica::moveObject(_Octree);
		scene->drawScene();

		glfwSwapBuffers(window);

	}

	scene.reset();
	_Octree.reset();

	//scene.deleteScene();

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

/*void ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset) {
	camera.processMouseScroll(yOffset);
}*/