#include "Directional_Light.h"

void DirectionalLightSetter::SetDirectionalLight(ShaderLoader shader) {
	//directional light
	shader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.SetVec3("dirLight.position", 20.0f, 100.0f, 30.0f);
	shader.SetVec3("dirLight.ambient", 0.15f, 0.15f, 0.15f);
	shader.SetVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
	shader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
	shader.Set1i("enableDir", 1);
}