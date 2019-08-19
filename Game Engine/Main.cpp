#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Scene/Scene.h"
#include "Collision Detection/Octree/Octree.h"
#include "Collision Detection/OctreeTester/OctreeTester.h"
#include "Fisica/Fisica.h"
#include "Shader/ShaderLoader.h"

const GLint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//funzioni per la gestione dei comandi
void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mode);
//void ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallBack(GLFWwindow *window, double xPos, double yPos);
void MouseButtonCallBack(GLFWwindow* window, int key, int action, int mods);
void DoMovement();

GLuint sceneNumber = 0;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true, timeStart = true, firstDelta = true;
bool pause = false, mouseEnabled = true;
bool showOctree = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool cameraMode = true;
Camera camera;
Fisica _Fisica;

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
	bounds.position = glm::vec3(0.0f, 0.0f, 0.0f);
	bounds.size = 20.0f;
	std::shared_ptr<Octree> _Octree(new Octree(NULL, bounds, 4, 0));

	camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2.0f, &cameraMode);

	glfwMakeContextCurrent(window);
	//assegnazione delle funzioni per la gestione dei comandi
	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetCursorPosCallback(window, MouseCallBack);
	glfwSetMouseButtonCallback(window, MouseButtonCallBack);
	//glfwSwapInterval(0);
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

	std::shared_ptr<Scene> scene(new Scene(camera));
	scene->loadScene(sceneNumber, _Octree);

	MeshStruct MS = _Octree->calculateVertices();
	OctreeTester OT(MS);

	GLint frames = 0;
	//loop dove vengono disegnati gli oggetti e gestiti i comandi

	GLfloat lastTime = glfwGetTime();

	ShaderLoader* octreeShader = new ShaderLoader("../Game Engine/GLSL/octree.vs", "../Game Engine/GLSL/octree.frag");
	while (!glfwWindowShouldClose(window)) {
		//delta time rappresenta la durata dell'ultimo frame, moltiplicando ad esempio la velcità con cui si sposta un oggetto nel tempo per il delta time, si rende il movimento indipendente dal tempo impiegato
		//per la creazione del frame

		glm::vec3 cameraMovement(0.0f);

		frames++;
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		if (currentFrame - lastTime >= 1.0) {
			std::cout << 1000.0 / (GLfloat)frames << std::endl;
			frames = 0;
			lastTime += 1.0;
		}
		lastFrame = currentFrame;

		if (firstDelta) {
			firstDelta = false;
			continue;
		}

		if (!firstDelta) {

			glfwPollEvents();
			//applicazione dei movimenti
			DoMovement();

			//render
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!pause) {
				_Fisica.moveObject(_Octree, deltaTime, cameraMovement, camera.getFront(), mouseEnabled);
				if (cameraMode) camera.updatePosition();
			}

			scene->drawScene();

			if (showOctree) {
				MS = _Octree->calculateVertices();
				OT.updateTester(MS);
				OT.DrawNoIndices(octreeShader, camera);
			}

			glfwSwapBuffers(window);

		}
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void DoMovement() {
	if (!cameraMode) {
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
}
//gestisce solo il comando esc per chiudere la finestra
void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (GLFW_KEY_P == key && GLFW_RELEASE == action) {
		pause = !pause;
	}

	if (GLFW_KEY_H == key && GLFW_RELEASE == action) {
		showOctree = !showOctree;
	}

	if (GLFW_KEY_X == key && GLFW_RELEASE == action) {
		_Fisica.changeDirection(1);
	}

	if (GLFW_KEY_Y == key && GLFW_RELEASE == action) {
		_Fisica.changeDirection(2);
	}

	if (GLFW_KEY_Z == key && GLFW_RELEASE == action) {
		_Fisica.changeDirection(3);
	}

	if (GLFW_KEY_F == key && GLFW_RELEASE == action) {
		cameraMode = !cameraMode;
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

void MouseButtonCallBack(GLFWwindow* window, int key, int action, int mods) {
	if (mouseEnabled) {
		if (timeStart && key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			_Fisica.setStartTime(glfwGetTime());
			timeStart != timeStart;
		}

		if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			_Fisica.setEndTime(glfwGetTime());
			timeStart != timeStart;
		}
	}
}