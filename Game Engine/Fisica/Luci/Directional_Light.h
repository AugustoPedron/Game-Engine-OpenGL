#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Shader/Shader.h"
#include "../../Camera/Camera.h"

namespace DirectionalLightSetter {
	void SetDirectionalLight(Shader shader);
}