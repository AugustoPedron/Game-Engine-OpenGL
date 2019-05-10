#include "Point_Light.h"

void PointLightSetter::SetPointLight(Shader shader, glm::vec3 position, GLuint index) {
	std::string pre, mid, positio, ambient, diffuse, specular, constant, linear, quadratic;
	std::stringstream ss;
	ss << index;
	pre = "pointLights[";
	positio = "].position";
	ambient = "].ambient";
	diffuse = "].diffuse";
	specular = "].specular";
	constant = "].constant";
	linear="].linear";
	quadratic = "].quadratic";
	mid = ss.str();

	//point light
	shader.SetVec3((pre + mid + positio).c_str(), position.x, position.y, position.z);
	shader.SetVec3((pre + mid + ambient).c_str(), 0.05f, 0.05f, 0.05f);
	shader.SetVec3((pre + mid + diffuse).c_str(), 0.8f, 0.8f, 0.8f);
	shader.SetVec3((pre + mid + specular).c_str(), 1.0f, 1.0f, 1.0f);
	shader.Set1f((pre + mid + constant).c_str(), 1.0f);
	shader.Set1f((pre + mid + linear).c_str(), 0.09f);
	shader.Set1f((pre + mid + quadratic).c_str(), 0.032f);
	shader.Set1i("enablePoint", 1);
}