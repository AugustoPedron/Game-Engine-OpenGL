#include "Shadows.h"
/*
ShadowSetter::Shadow::Shadow(GLuint TextureID, GLuint VAO) {
	this->textureID = TextureID;
	this->VAO = VAO;
	//this->lightSpaceMatrix = lightSpaceMatrix;
	this->shader = std::make_shared<ShaderLoader>("../Game Engine/GLSL/shadowShader.vs", "../Game Engine/GLSL/shadowShader.frag");
	this->SetShadow();
}

void ShadowSetter::Shadow::SetShadow() {
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowSetter::Shadow::DrawShadows(glm::mat4 model) {
	this->shader->use();

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
	lightView = glm::lookAt(glm::vec3(20000.0f, 100000.0f, 30000.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	this->shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	this->shader->SetMat4("model", model);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int ShadowSetter::Shadow::getDepthMap() {
	return this->depthMap;
}

unsigned int ShadowSetter::Shadow::getDepthMapFBO() {
	return this->depthMapFBO;
}*/