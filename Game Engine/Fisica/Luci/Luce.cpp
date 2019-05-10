#include "Luce.h"

void LightSetter::SetLight(Shader shader, Camera camera, glm::mat4 projection, glm::vec3 lightColor, glm::vec3 position) {
	glm::mat4 view = camera.GetViewMatrix();
	GLint modelLoc = glGetUniformLocation(shader.program, "model");
	GLint viewLoc = glGetUniformLocation(shader.program, "view");
	GLint projectionLoc = glGetUniformLocation(shader.program, "projection");
	GLint lampColorLoc = glGetUniformLocation(shader.program, "lightColor");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform4f(lampColorLoc, lightColor.x, lightColor.y, lightColor.z, 1.0f);

	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.2f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}