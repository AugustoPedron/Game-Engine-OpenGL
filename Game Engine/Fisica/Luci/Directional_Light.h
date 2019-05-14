#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Shader/ShaderLoader.h"

namespace DirectionalLightSetter {
	void SetDirectionalLight(ShaderLoader shader);
}