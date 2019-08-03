#include "Fisica.h"

void Fisica::moveObject(std::shared_ptr<Octree> Octree) {
	std::shared_ptr<ModelLoader> ML = Octree->getML(0);
	if (ML->getPositionPhy().y + 0.5f + 0.005f < 10.0f) {
		ML->updatePosition(glm::vec3(0.0f, +0.005f, 0.0f));
		Octree->updateOctree(0);
	}
	ML.reset();
}