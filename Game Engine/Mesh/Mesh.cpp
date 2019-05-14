#include "Mesh.h"

Mesh::Mesh(std::vector<MeshStruct> Meshes) {
	std::vector<MeshStruct>::iterator it = Meshes.begin();
	int i = 0;

	for (it; it != Meshes.end(); it++, i++) {
		MeshLoader m(Meshes[i].Vertices, Meshes[i].texture_diffuse.c_str(), Meshes[i].texture_specular.c_str());
		this->Meshes.push_back(m);
	}
}

void Mesh::Draw(ShaderLoader shaderLoader, std::vector<glm::mat4> meshesPositions, Camera &camera) {
	shaderLoader.use();
	shaderLoader.SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shaderLoader.SetMat4("projection", camera.getProjection());
	shaderLoader.SetMat4("view", camera.GetViewMatrix());
	DirectionalLightSetter::SetDirectionalLight(shaderLoader);
	std::vector<MeshLoader>::iterator it = this->Meshes.begin();
	GLuint i = 0;
	for (it; it != this->Meshes.end(); it++, i++) {
		shaderLoader.SetMat4("model", meshesPositions[i]);
		this->Meshes[i].DrawNoIndices(shaderLoader);
	}
}

void Mesh::DeleteMeshes() {
	std::vector<MeshLoader>::iterator it = this->Meshes.begin();

	for (it; it != this->Meshes.end(); it++) {
		it->DeleteMesh();
	}
}