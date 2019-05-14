#include "Skybox.h"

Skybox::Skybox(SkyboxStruct Skybox) {
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Skybox.Vertices.size(), &Skybox.Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);

	this->skyboxTexture = TextureLoader::LoadSkyboxMap(Skybox.Textures);
}

void Skybox::Draw(ShaderLoader shader, Camera &camera) {
	glDepthFunc(GL_LEQUAL);

	shader.use();

	shader.Set1i("skybox", 0);
	shader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
	shader.SetMat4("projection", camera.getProjection());
	glBindVertexArray(this->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

void Skybox::DeleteSkybox() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VAO);
}