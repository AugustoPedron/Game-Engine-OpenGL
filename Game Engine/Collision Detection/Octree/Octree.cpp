#include "Octree.h"

#pragma region Octree

Octree::Octree(Octree *parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth)
{
	this->parent = parent;
	this->maxDepth = maxDepth;
	this->Bounds = Bounds;
	this->depth = depth;
	for (GLuint i = 0; i < 8; i++) {
		this->alreadyCreated[i] = 0;
	}
}

void Octree::addObject(ModelLoader& model)
{
	GLuint i = 0, k = 0;
	VertexOctree vertexTemp;
	IndexOctree indexTemp;
	std::vector<VertexOctree> verticesToAdd;
	std::vector<IndexOctree> indicesToAdd;
	std::vector<MeshLoader>::iterator it2 = model.getMeshes().begin();
	for (it2; it2 != model.getMeshes().end(); it2++, k++) {
		this->vertices.push_back(it2->getVertices());
		std::vector<GLuint>::iterator it = it2->getIndices().begin();
		for (it; it != it2->getIndices().end(); it += 3) {
			for (GLuint j = 0; j < 3; j++, i++) {
				vertexTemp.Position = this->vertices.back().get()[it2->getIndices()[i]].Position;
				vertexTemp.Normal = this->vertices.back().get()[it2->getIndices()[i]].Normal;
				verticesToAdd.push_back(vertexTemp);

				indexTemp.index = it2->getIndices()[i];
				indicesToAdd.push_back(indexTemp);
			}
		}
		insertRecursive(verticesToAdd, indicesToAdd, 0, this->vertices.size() - 1, model, this->vertices);
	}

}

bool Octree::insertRecursive(std::vector<VertexOctree>& vertices, std::vector<IndexOctree>& indices, GLuint depth, GLuint meshNumber, ModelLoader& model, std::vector<std::reference_wrapper<std::vector<Vertex>>> verticesRef) {
	this->ModelsIdList.push_back(model.getId());
	int childFlag[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	if (depth > this->maxDepth) return true;

	for (GLuint i = 0; i < indices.size(); i++) {
		indices[i].code = 0;
		if (vertices[indices[i].index].Position.x * model.getPositionPhy().x > this->Bounds.position.x) indices[i].code |= 1;
		if (vertices[indices[i].index].Position.y * model.getPositionPhy().y > this->Bounds.position.y) indices[i].code |= 2;
		if (vertices[indices[i].index].Position.z * model.getPositionPhy().z > this->Bounds.position.z) indices[i].code |= 4;

		if ((i + 1) % 3 == 0) {
			if (indices[i - 2].code == indices[i - 1].code && indices[i - 2].code == indices[i].code) {
				childFlag[indices[i].code]++;
				indices[i].flag = 1;
				indices[i - 2].flag = 1;
				indices[i - 1].flag = 1;
			}
			else {
				this->triangles.emplace_back(meshNumber, indices[i - 2].index, indices[i - 1].index, indices[i].index, model, verticesRef);
			}
		}
	}

	std::vector<IndexOctree> indicesTemp;

	for (GLuint i = 0; i < 8; i++) {
		if (!childFlag[i]) continue;
		if (!this->alreadyCreated[i]) {
			OctreeBounds newBounds = this->Bounds;
			newBounds.position.x += newBounds.size * offsets[i].x;
			newBounds.position.y += newBounds.size * offsets[i].y;
			newBounds.position.z += newBounds.size * offsets[i].z;
			newBounds.size *= 0.5;
			this->alreadyCreated[i] = 1;
			this->children[i] = new Octree(this, newBounds, this->maxDepth, depth + 1);
		}

		indicesTemp.clear();

		for (GLuint j = 0; j < indices.size(); j += 3) {
			if (indices[j].flag == 1 && indices[j].code == i && indices[j + 1].flag == 1 && indices[j + 1].code == i && indices[j + 2].flag == 1 && indices[j + 2].code == i) {
				indicesTemp.push_back(indices[j]);
				indicesTemp.push_back(indices[j + 1]);
				indicesTemp.push_back(indices[j + 2]);
			}
		}

		if (this->children[i]->insertRecursive(vertices, indicesTemp, depth + 1, meshNumber, model, verticesRef)) {
			for (GLuint k = 0; k < indicesTemp.size(); k += 3) {
				this->triangles.emplace_back(meshNumber, indicesTemp[k].index, indicesTemp[k + 1].index, indicesTemp[k + 2].index, model, verticesRef);
			}
		}
	}

	return false;
}

void Octree::updateOctree(GLuint id) {
	GLuint flag = 0;
	std::vector<GLuint>::iterator it = this->ModelsIdList.begin();
	for (it; it != this->ModelsIdList.end(); it++) {
		if (*it == id) {
			flag = 1;
			if (this->triangles.size() > 0) {
				std::vector<OctreeNodeTriangle>::iterator it2 = this->triangles.begin();
				for (it2; it2 != this->triangles.end(); it2++) {
					if (it2->ML.getId() == id && this->checkBoundingBox(*it2)) {
						OctreeNodeTriangle temp = *it2;
						this->triangles.erase(it2);
						this->moveTrianglesUp(temp);
						it2--;
					}
				}
			}
		}
	}
	for (GLuint i = 0; i < 8; i++) {
		if (this->alreadyCreated[i]) this->children[i]->updateOctree(id);
	}

	if (!flag) return;


}

bool Octree::checkBoundingBox(OctreeNodeTriangle& triangle) {
	for (GLuint i = 0; i < 3; i++) {
		if ((triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.x * triangle.ML.getPositionPhy().x) > (Bounds.position.x + Bounds.size)) return true;
		if ((triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.x * triangle.ML.getPositionPhy().x) < (Bounds.position.x - Bounds.size)) return true;
		if ((triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.y * triangle.ML.getPositionPhy().y) > (Bounds.position.y + Bounds.size)) return true;
		if ((triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.y * triangle.ML.getPositionPhy().y) < (Bounds.position.y - Bounds.size)) return true;
		if ((triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.z * triangle.ML.getPositionPhy().z) > (Bounds.position.z + Bounds.size)) return true;
		if ((triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.z * triangle.ML.getPositionPhy().z) < (Bounds.position.z - Bounds.size)) return true;
	}
	return false;
}

void Octree::moveTrianglesUp(OctreeNodeTriangle &triangle) {
	if (this->parent != NULL) {
		if (this->parent->checkBoundingBox(triangle)) {
			this->parent->moveTrianglesUp(triangle);
		}
		else {
			this->parent->trianglesToMove.push_back(triangle);
		}
	}
	else {
		this->trianglesToMove.push_back(triangle);
	}
	this->moveTriangleDown();
}

void Octree::moveTriangleDown() {
	std::vector<OctreeNodeTriangle>::iterator it = this->trianglesToMove.begin();
	for (it; it != trianglesToMove.end(); it++) {
		this->insertRecursive(*it, this->depth);
	}
	this->trianglesToMove.clear();
}

bool Octree::insertRecursive(OctreeNodeTriangle& triangle, GLuint depth) {
	this->ModelsIdList.push_back(triangle.ML.getId());

	if (depth > this->maxDepth) return true;

	std::vector<GLuint> code;
	for (GLuint i = 0; i < 3; i++) {
		code[i] = 0;
		if (triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.x * triangle.ML.getPositionPhy().x > this->Bounds.position.x) code[i] |= 1;
		if (triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.y * triangle.ML.getPositionPhy().y > this->Bounds.position.y) code[i] |= 2;
		if (triangle.verticesRef.get()[triangle.meshNumber].get()[triangle.indices[i]].Position.z * triangle.ML.getPositionPhy().z > this->Bounds.position.z) code[i] |= 4;

		if ((i + 1) % 3 == 0) {
			if (code[i - 2] == code[i - 1] && code[i - 2] == code[i]) {
				if (!this->alreadyCreated[code[i]]) {
					OctreeBounds newBounds = this->Bounds;
					newBounds.position.x += newBounds.size * offsets[i].x;
					newBounds.position.y += newBounds.size * offsets[i].y;
					newBounds.position.z += newBounds.size * offsets[i].z;
					newBounds.size *= 0.5;
					this->alreadyCreated[code[i]] = 1;
					this->children[code[i]] = new Octree(this, newBounds, this->maxDepth, depth + 1);
				}

				if (this->children[code[i]]->insertRecursive(triangle, depth + 1)) {
					this->triangles.push_back(triangle);
				}
			}
			else {
				this->triangles.push_back(triangle);
			}
		}
	}

	return false;
}

#pragma endregion

#pragma region OctreeNodeMesh

Octree::OctreeNodeTriangle::OctreeNodeTriangle(GLuint meshNumber, GLuint index1, GLuint index2, GLuint index3, ModelLoader& ML, std::vector<std::reference_wrapper<std::vector<Vertex>>> verticesRef) : ML(ML), verticesRef(verticesRef) {
	this->meshNumber = meshNumber;
	this->indices.push_back(index1);
	this->indices.push_back(index2);
	this->indices.push_back(index3);
}

Octree::OctreeNodeTriangle& Octree::OctreeNodeTriangle::operator= (const OctreeNodeTriangle &ref) {
	if (this != &ref) {
		this->meshNumber = ref.meshNumber;
		this->ML = ref.ML;
		this->verticesRef = ref.verticesRef;
		this->indices.push_back(ref.indices[0]);
		this->indices.push_back(ref.indices[1]);
		this->indices.push_back(ref.indices[2]);
	}
	return *this;
}
#pragma endregion