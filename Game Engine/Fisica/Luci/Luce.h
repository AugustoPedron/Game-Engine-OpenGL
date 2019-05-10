#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Shader/Shader.h"
#include "../../Camera/Camera.h"

namespace LightSetter {
	void SetLight(Shader shader, Camera camera, glm::mat4 projection, glm::vec3 lightColor, glm::vec3 position);
}