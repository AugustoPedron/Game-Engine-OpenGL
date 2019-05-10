#include "Directional_Light.h"

void DirectionalLightSetter::SetDirectionalLight(Shader shader) {
	//directional light
	shader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.SetVec3("dirLight.position", 20000.0f, 100000.0f, 30000.0f);
	shader.SetVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
	shader.SetVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	shader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
	shader.Set1i("enableDir", 1);
}