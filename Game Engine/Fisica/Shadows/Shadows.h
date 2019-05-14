#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/types.h>

namespace ShadowSetter {
	/*
	const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	//inline int getDepthMapFBO() { return depthMapFBO; }
	//inline int getDepthMap() { return depthMap; }
	
	class Shadow {
	private:
		unsigned int depthMapFBO;
		unsigned int depthMap;
		GLuint VAO;
		GLuint textureID;
		std::shared_ptr<ShaderLoader> shader;
		void SetShadow();

	public:
		Shadow(GLuint textureID, GLuint VAO);
		void DrawShadows(glm::mat4 model);
		inline int getShadowWidth() { return SHADOW_WIDTH; }
		inline int getShadowHeight() { return SHADOW_HEIGHT; }
		//inline void setTextureID(GLuint texID) { textureID = texID; }
		unsigned int getDepthMap();
		unsigned int getDepthMapFBO();
	};
*/
}