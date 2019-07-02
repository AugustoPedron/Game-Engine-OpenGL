#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Shader/ShaderLoader.h"

namespace SpotLightSetter {
	void SetSpotLight(ShaderLoader shader, glm::vec3 position, glm::vec3 direction);
}