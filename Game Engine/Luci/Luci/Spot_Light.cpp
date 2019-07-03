#include "Spot_Light.h"

void SpotLightSetter::SetSpotLight(ShaderLoader shader, glm::vec3 position, glm::vec3 direction) {
	shader.SetVec3("spotLight.position", position.x, position.y, position.z);
	shader.SetVec3("spotLight.direction", direction.x, direction.y, direction.z);
	shader.SetVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	shader.SetVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
	shader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.Set1f("spotLight.constant", 1.0f);
	shader.Set1f("spotLight.linear", 0.09f);
	shader.Set1f("spotLight.quadratic", 0.032f);
	shader.Set1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.Set1f("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	shader.Set1i("enableSpot", 1);
}