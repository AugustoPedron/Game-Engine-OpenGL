#include "Shadows.h"

Shadow::Shadow() {
	glGenFramebuffers(1, &this->depthMapFBO);
	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void Shadow::DrawShadows(ShaderLoader* shadowShader, Mesh* Meshes, Model* Models, Camera &camera) {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	shadowShader->use();
	shadowShader->SetMat4("lightSpaceMatrix", camera.getLightSpaceMatrix());

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	GLuint i = 0;
	if (!Meshes->getMeshes().empty()) {
		std::vector<MeshLoader>::iterator it = Meshes->getMeshes().begin();
		for (it; it != Meshes->getMeshes().end(); it++, i++) {
			shadowShader->SetMat4("model", it->getPosition());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, it->getDiffuseMap());
			glBindVertexArray(it->getVAO());
			if (i == 1) {
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else {
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			
			glBindVertexArray(0);
		}
	}

	GLuint k = 0;
	if (!Models->getModels().empty()) {
		std::map<const GLchar*, ModelLoader>::iterator it2 = Models->getModels().begin();
		for (it2; it2 != Models->getModels().end(); it2++, k++) {
			shadowShader->SetMat4("model", it2->second.getPosition());
			for (GLuint j = 0; j < it2->second.getMeshes().size(); j++) {
				GLuint diffuseNr = 1;
				for (i = 0; i < it2->second.getMeshes()[j].getTextures().size(); i++) {
					if ("texture_diffuse" == it2->second.getMeshes()[j].getTextures()[i].type) {
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, it2->second.getMeshes()[j].getTextures()[i].id);
					}
				}

				glBindVertexArray(it2->second.getMeshes()[j].getVAO());
				glDrawElements(GL_TRIANGLES, it2->second.getMeshes()[j].getIndices().size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				for (GLuint i = 0; i < it2->second.getMeshes()[j].getTextures().size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::DrawShadowsDebug(ShaderLoader shadowShader) {
	shadowShader.use();
	shadowShader.Set1i("depthMap", 0);
	shadowShader.Set1f("near_plane", 1.0f);
	shadowShader.Set1f("far_plane", 150.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}