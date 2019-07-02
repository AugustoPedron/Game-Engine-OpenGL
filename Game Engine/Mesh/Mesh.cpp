#include "Mesh.h"

Mesh::Mesh(std::vector<MeshDefinition> Meshes) {
	std::vector<MeshDefinition>::iterator it = Meshes.begin();
	int i = 0;

	for (it; it != Meshes.end(); it++, i++) {
		MeshLoader m(Meshes[i]);
		this->Meshes.push_back(m);
	}
}

void Mesh::Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap) {
	shaderLoader->use();
	shaderLoader->SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shaderLoader->SetMat4("projection", camera.getProjection());
	shaderLoader->SetMat4("view", camera.GetViewMatrix());
	shaderLoader->SetMat4("lightSpaceMatrix", camera.getLightSpaceMatrix());
	DirectionalLightSetter::SetDirectionalLight(shaderLoader);
	std::vector<MeshLoader>::iterator it = this->Meshes.begin();
	GLuint i = 0;
	for (it; it != this->Meshes.end(); it++, i++) {
		this->Meshes[i].DrawNoIndices(shaderLoader, shadowMap);
	}
}

void Mesh::DeleteMeshes() {
	std::vector<MeshLoader>::iterator it = this->Meshes.begin();

	for (it; it != this->Meshes.end(); it++) {
		it->DeleteMesh();
	}
}