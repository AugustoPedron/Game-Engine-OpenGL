#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Shader/Shader.h"
#include "../../Camera/Camera.h"

namespace SpotLightSetter {
	void SetSpotLight(Shader shader, glm::vec3 position, glm::vec3 direction);
}