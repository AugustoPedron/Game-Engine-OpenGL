#include "Octree.h"

#pragma region Octree

Octree::Octree(std::shared_ptr<Octree> parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth)
{

}

Octree::Octree(Octree* parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth) {
	this->parent = parent;
	this->maxDepth = maxDepth;
	this->Bounds = Bounds;
	this->depth = depth;
	this->verticesReferences = std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>>(new std::vector<std::shared_ptr<std::vector<Vertex>>>());
	for (GLuint i = 0; i < 8; i++) {
		this->alreadyCreated[i] = 0;
	}
}

void Octree::addObject(std::shared_ptr<ModelLoader> model)
{
	GLuint i = 0, k = 0;
	IndexOctree indexTemp;
	std::vector<VertexOctree> verticesToAdd;
	std::vector<IndexOctree> indicesToAdd;
	std::vector<std::shared_ptr<MeshLoader>>::iterator it2 = model->getMeshes().begin();
	for (it2; it2 != model->getMeshes().end(); it2++, k++) {
		i = 0;
		this->verticesReferences->push_back((*it2)->getVertices());
		std::vector<GLuint>::iterator it = model->getMeshes().at(k)->getIndices().begin();
		for (it; it != model->getMeshes().at(k)->getIndices().end(); it++) {
			indexTemp.index = *it;
			indicesToAdd.push_back(indexTemp);
		}
		insertRecursive(indicesToAdd, 0, this->verticesReferences->size() - 1, model, this->verticesReferences);
	}

}

void Octree::insertRecursive(std::vector<IndexOctree>& indices, GLuint depth, GLuint meshNumber, std::shared_ptr<ModelLoader> model, std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>> verticesRef) {
	this->ModelsIdList.insert(std::pair<GLuint, std::shared_ptr<ModelLoader>>(model->getId(), model));

	this->verticesReferences = std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>>(verticesRef);

	int childFlag[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	for (GLuint i = 0; i < indices.size(); i++) {
		indices[i].code = 0;
		if (this->verticesReferences->at(meshNumber)->at(indices[i].index).Position.x + model->getPositionPhy().x > this->Bounds.position.x) indices[i].code |= 1;
		if (this->verticesReferences->at(meshNumber)->at(indices[i].index).Position.y + model->getPositionPhy().y > this->Bounds.position.y) indices[i].code |= 2;
		if (this->verticesReferences->at(meshNumber)->at(indices[i].index).Position.z + model->getPositionPhy().z > this->Bounds.position.z) indices[i].code |= 4;

		if ((i + 1) % 3 == 0) {
			if (indices[i - 2].code == indices[i - 1].code && indices[i - 2].code == indices[i].code) {
				childFlag[indices[i].code]++;
				indices[i].flag = 1;
				indices[i - 2].flag = 1;
				indices[i - 1].flag = 1;
			}
			else {
				this->triangles.emplace_back(meshNumber, indices[i - 2].index, indices[i - 1].index, indices[i].index, model, this->verticesReferences);
				/*if (this->ModelsCounter.size() == 0) {
					OctreeStruct t;
					t.ID = meshNumber;
					t.counter = 1;
					this->ModelsCounter.push_back(t);
				}
				else {

				}*/
			}
		}
	}

	std::vector<IndexOctree> indicesTemp;

	if (depth + 1 <= this->maxDepth) {
		for (GLuint i = 0; i < 8; i++) {
			if (!childFlag[i]) continue;
			if (!this->alreadyCreated[i]) {
				OctreeBounds newBounds = this->Bounds;
				newBounds.position.x += newBounds.size * offsets[i].x;
				newBounds.position.y += newBounds.size * offsets[i].y;
				newBounds.position.z += newBounds.size * offsets[i].z;
				newBounds.size *= 0.5;
				this->alreadyCreated[i] = 1;
				this->children[i] = std::make_shared<Octree>(this, newBounds, this->maxDepth, depth + 1);
			}

			indicesTemp.clear();

			for (GLuint j = 0; j < indices.size(); j += 3) {
				if (indices[j].flag == 1 && indices[j].code == i && indices[j + 1].flag == 1 && indices[j + 1].code == i && indices[j + 2].flag == 1 && indices[j + 2].code == i) {
					indicesTemp.push_back(indices[j]);
					indicesTemp.push_back(indices[j + 1]);
					indicesTemp.push_back(indices[j + 2]);
				}
			}

			this->children[i]->insertRecursive(indicesTemp, depth + 1, meshNumber, model, verticesRef);
		}
	}
	else {
		for (GLuint k = 0; k < indices.size(); k += 3) {
			this->triangles.emplace_back(meshNumber, indices[k].index, indices[k + 1].index, indices[k + 2].index, model, this->verticesReferences);
		}
	}
}

void Octree::updateOctree(GLuint id) {
	GLuint flag = 0;
	GLuint flagDelete = 0;
	GLuint i = 0;
	if (this->ModelsIdList.find(id) != this->ModelsIdList.end()) {
		flag = 1;
		if (this->triangles.size() > 0) {
			std::vector<OctreeNodeTriangle>::iterator it2 = this->triangles.begin();
			for (it2; it2 != this->triangles.end();) {

				if (it2->ML->getId() == id) {
					if (this->parent != NULL && this->checkBoundingBox(*it2)) {
						OctreeNodeTriangle temp = *it2;
						it2 = this->triangles.erase(it2);
						flagDelete = 1;
						this->moveTrianglesUp(temp);
					}
					else if (it2->ML->getMovingCondition() == 1) {
						GLuint result = this->checkChildrenBoundingBox(*it2);
						if (result < 8 && this->depth < maxDepth) {
							OctreeNodeTriangle temp = *it2;
							it2 = this->triangles.erase(it2);
							flagDelete = 1;
							this->insertRecursive(temp, this->depth, result);
							break;
						}
					}
	
				}

				if (!flagDelete) {
					it2++;
					i++;
				}
				else {
					flagDelete = 0;
				}
			}
		}
	}

	if (!flag) return;

	for (GLuint i = 0; i < 8; i++) {
		if (this->alreadyCreated[i]) {
			this->children[i]->updateOctree(id);
		}
	}
}

bool Octree::checkBoundingBox(OctreeNodeTriangle& triangle) {
	if (!this->checkParentBundingBox(triangle)) {
		for (GLuint i = 0; i < 3; i++) {
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.x + triangle.ML->getPositionPhy().x > (this->Bounds.position.x + this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.x + triangle.ML->getPositionPhy().x < (this->Bounds.position.x - this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.y + triangle.ML->getPositionPhy().y > (this->Bounds.position.y + this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.y + triangle.ML->getPositionPhy().y < (this->Bounds.position.y - this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.z + triangle.ML->getPositionPhy().z > (this->Bounds.position.z + this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.z + triangle.ML->getPositionPhy().z < (this->Bounds.position.z - this->Bounds.size / 2)) return true;
		}
	}
	else {
		//if (triangle.ML->getMovingCondition()) triangle.ML->revertLastTranslation();
	}
	return false;
}

bool Octree::checkParentBundingBox(OctreeNodeTriangle& triangle) {
	if (this->parent != NULL) return this->parent->checkParentBundingBox(triangle);
	else {
		for (GLuint i = 0; i < 3; i++) {
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.x + triangle.ML->getPositionPhy().x > (this->Bounds.position.x + this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.x + triangle.ML->getPositionPhy().x < (this->Bounds.position.x - this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.y + triangle.ML->getPositionPhy().y > (this->Bounds.position.y + this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.y + triangle.ML->getPositionPhy().y < (this->Bounds.position.y - this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.z + triangle.ML->getPositionPhy().z > (this->Bounds.position.z + this->Bounds.size / 2)) return true;
			if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.z + triangle.ML->getPositionPhy().z < (this->Bounds.position.z - this->Bounds.size / 2)) return true;
		}
	}

	return false;
}

GLuint Octree::checkChildrenBoundingBox(OctreeNodeTriangle& triangle) {
	GLuint code[3];
	for (GLuint i = 0; i < 3; i++) {
		code[i] = 0;
		if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.x + triangle.ML->getPositionPhy().x > this->Bounds.position.x) code[i] |= 1;
		if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.y + triangle.ML->getPositionPhy().y > this->Bounds.position.y) code[i] |= 2;
		if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.z + triangle.ML->getPositionPhy().z > this->Bounds.position.z) code[i] |= 4;
	}

	if (code[0] == code[1] && code[0] == code[2])  return code[0];
	return 8;
}

void Octree::moveTrianglesUp(OctreeNodeTriangle &triangle) {
	if (this->parent != NULL) {
		if (this->parent->checkBoundingBox(triangle)) {
			this->parent->moveTrianglesUp(triangle);
		}
		else {
			this->parent->insertRecursive(triangle, this->parent->depth);
		}
	}
	else {
		this->insertRecursive(triangle, this->depth);
	}
}

bool Octree::insertRecursive(OctreeNodeTriangle& triangle, GLuint depth) {
	if (depth > this->maxDepth) return true;

	if (this->ModelsIdList.find(triangle.ML->getId()) == this->ModelsIdList.end())
		this->ModelsIdList.insert(std::pair<GLuint, std::shared_ptr<ModelLoader>>(triangle.ML->getId(), triangle.ML));

	this->verticesReferences = triangle.verticesRef;

	GLuint code[3];
	for (GLuint i = 0; i < 3; i++) {
		code[i] = 0;
		if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.x + triangle.ML->getPositionPhy().x > this->Bounds.position.x) code[i] |= 1;
		if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.y + triangle.ML->getPositionPhy().y > this->Bounds.position.y) code[i] |= 2;
		if (triangle.verticesRef->at(triangle.meshNumber)->at(triangle.indices[i]).Position.z + triangle.ML->getPositionPhy().z > this->Bounds.position.z) code[i] |= 4;

		if ((i + 1) % 3 == 0) {
			if (code[i - 2] == code[i - 1] && code[i - 2] == code[i]) {
				if (!this->alreadyCreated[code[i]]) {
					OctreeBounds newBounds = this->Bounds;
					newBounds.position.y += newBounds.size * offsets[code[i]].y;
					newBounds.position.x += newBounds.size * offsets[code[i]].x;
					newBounds.position.z += newBounds.size * offsets[code[i]].z;
					newBounds.size *= 0.5;
					this->alreadyCreated[code[i]] = 1;
					this->children[code[i]] = std::make_shared<Octree>(this, newBounds, this->maxDepth, depth + 1);
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

void Octree::insertRecursive(OctreeNodeTriangle& triangle, GLuint depth, GLuint child) {
	if (this->ModelsIdList.find(triangle.ML->getId()) == this->ModelsIdList.end())
		this->ModelsIdList.insert(std::pair<GLuint, std::shared_ptr<ModelLoader>>(triangle.ML->getId(), triangle.ML));

	this->verticesReferences = triangle.verticesRef;

	if (!this->alreadyCreated[child]) {
		OctreeBounds newBounds = this->Bounds;
		newBounds.position.x += newBounds.size * offsets[child].x;
		newBounds.position.y += newBounds.size * offsets[child].y;
		newBounds.position.z += newBounds.size * offsets[child].z;
		newBounds.size *= 0.5;
		this->alreadyCreated[child] = 1;
		this->children[child] = std::make_shared<Octree>(this, newBounds, this->maxDepth, depth + 1);
	}

	this->children[child]->insertRecursive(triangle, depth + 1);
}

bool Octree::checkCollisionRecursive(GLuint id, glm::vec3& direction) {
	for (GLuint i = 0; i < 8; i++) {
		if (this->alreadyCreated[i] == 1 && this->children[i]->ModelsIdList.find(id) != this->children[i]->ModelsIdList.end()) {
			if (this->children[i]->checkCollisionRecursive(id, direction)) {
				return true;
			}
		}
	}

	std::vector<OctreeNodeTriangle>::iterator it = this->triangles.begin();
	for (it; it != this->triangles.end(); it++) {
		if (it->ML->getId() == id) {
			if (this->checkCollisionRecursive(id, *it, direction)) {
				return true;
			}
		}
	}

	return false;
}

bool Octree::checkCollisionRecursive(GLuint id, OctreeNodeTriangle& triangleToCheck, glm::vec3& direction) {
	std::vector<OctreeNodeTriangle>::iterator it = this->triangles.begin();
	glm::vec3 A = triangleToCheck.verticesRef->at(triangleToCheck.meshNumber)->at(triangleToCheck.indices[0]).Position + triangleToCheck.ML->getPositionPhy();
	glm::vec3 B = triangleToCheck.verticesRef->at(triangleToCheck.meshNumber)->at(triangleToCheck.indices[1]).Position + triangleToCheck.ML->getPositionPhy();
	glm::vec3 C = triangleToCheck.verticesRef->at(triangleToCheck.meshNumber)->at(triangleToCheck.indices[2]).Position + triangleToCheck.ML->getPositionPhy();
	for (it; it != this->triangles.end(); it++) {
		if (it->ML->getId() != id) {
			glm::vec3 P = it->verticesRef->at(it->meshNumber)->at(it->indices[0]).Position;
			glm::vec3 Q = it->verticesRef->at(it->meshNumber)->at(it->indices[1]).Position;
			glm::vec3 R = it->verticesRef->at(it->meshNumber)->at(it->indices[2]).Position;
			P = glm::rotateY(P, glm::radians(it->ML->getRotation())) + it->ML->getPositionPhy();
			Q = glm::rotateY(Q, glm::radians(it->ML->getRotation())) + it->ML->getPositionPhy();
			R = glm::rotateY(R, glm::radians(it->ML->getRotation())) + it->ML->getPositionPhy();
			if (this->triangleTriangleIntersection(A, B, C, P, Q, R)) {
				glm::vec3 normal = glm::triangleNormal(P, Q, R);
				glm::vec3 newDir;
				newDir = glm::reflect(direction, normal);
				glm::vec3 centerToCenter = it->ML->getPositionPhy() - triangleToCheck.ML->getPositionPhy();
				if (centerToCenter.z > 0 && newDir.z > 0) {
					if (centerToCenter.x > 0 && newDir.x > 0) return false;
					if (centerToCenter.x < 0 && newDir.x < 0) return false;
				}
				else if (centerToCenter.z < 0 && newDir.z < 0) {
					if (centerToCenter.x > 0 && newDir.x > 0) return false;
					if (centerToCenter.x < 0 && newDir.x < 0) return false;
				}
				else {
					direction = newDir;
					return true;
				}
			}
		}
	}

	if (this->parent != NULL) return this->parent->checkCollisionRecursive(id, triangleToCheck, direction);

	return false;
}

bool Octree::checkSecondCollisionRecursive(GLuint id, OctreeNodeTriangle& triangleToCheck, OctreeNodeTriangle& firstTriangle, glm::vec3& direction) {
	std::vector<OctreeNodeTriangle>::iterator it = this->triangles.begin();
	glm::vec3 A = firstTriangle.verticesRef->at(firstTriangle.meshNumber)->at(firstTriangle.indices[0]).Position;
	glm::vec3 B = firstTriangle.verticesRef->at(firstTriangle.meshNumber)->at(firstTriangle.indices[1]).Position;
	glm::vec3 C = firstTriangle.verticesRef->at(firstTriangle.meshNumber)->at(firstTriangle.indices[2]).Position;
	A = glm::rotateY(A, glm::radians(firstTriangle.ML->getRotation())) + firstTriangle.ML->getPositionPhy();
	B = glm::rotateY(B, glm::radians(firstTriangle.ML->getRotation())) + firstTriangle.ML->getPositionPhy();
	C = glm::rotateY(C, glm::radians(firstTriangle.ML->getRotation())) + firstTriangle.ML->getPositionPhy();
	for (it; it != this->triangles.end(); it++) {
		if (it->ML->getId() != id) {
			glm::vec3 P = it->verticesRef->at(it->meshNumber)->at(it->indices[0]).Position;
			glm::vec3 Q = it->verticesRef->at(it->meshNumber)->at(it->indices[1]).Position;
			glm::vec3 R = it->verticesRef->at(it->meshNumber)->at(it->indices[2]).Position;
			P = glm::rotateY(P, glm::radians(it->ML->getRotation())) + it->ML->getPositionPhy();
			Q = glm::rotateY(Q, glm::radians(it->ML->getRotation())) + it->ML->getPositionPhy();
			R = glm::rotateY(R, glm::radians(it->ML->getRotation())) + it->ML->getPositionPhy();
			if (A != P && B != Q && C != R) {
				//if(A.z==)
				if (this->triangleTriangleIntersection(A, B, C, P, Q, R)) {
					glm::vec3 normal = glm::triangleNormal(P, Q, R);
					glm::vec3 dir = direction;
					direction = glm::reflect(direction, normal);
					return true;
				}
			}
		}
	}

	if (this->parent != NULL) return this->parent->checkCollisionRecursive(id, triangleToCheck, direction);

	return false;
}

bool Octree::triangleTriangleIntersection(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P, glm::vec3 Q, glm::vec3 R) {
	if (this->triangleSegmentIntersection(A, B, P, Q, R)) return true;
	else if (this->triangleSegmentIntersection(B, C, P, Q, R)) return true;
	else  if (this->triangleSegmentIntersection(C, A, P, Q, R)) return true;
	else return false;
}

bool Octree::triangleSegmentIntersection(glm::vec3 A, glm::vec3 B, glm::vec3 P, glm::vec3 Q, glm::vec3 R) {
	float epsilon = 0.00001;
	glm::vec3 a = A - R;
	glm::vec3 b = P - R;
	glm::vec3 c = Q - R;
	glm::vec3 w1 = cross(b, c);
	float w = dot(a, w1);
	glm::vec3 d = B - R;
	float s = dot(d, w1);
	if (w > epsilon) {
		if (s > epsilon) return false;
		glm::vec3 w2 = cross(a, d);
		float t = dot(w2, c);
		if (t < -epsilon) return false;
		float u = -dot(w2, b);
		if (u < -epsilon) return false;
		if (w < s + t + u) return false;
	}
	else if (w < -epsilon) {
		if (s < -epsilon) return false;
		glm::vec3 w2 = cross(a, d);
		float t = dot(w2, c);
		if (t > epsilon) return false;
		float u = -dot(w2, b);
		if (u > epsilon) return false;
		if (w > s + t + u) return false;
	}
	else {
		glm::vec3 temp = B;
		B = A;
		A = temp;
		if (s > epsilon) {
			glm::vec3 w2 = cross(d, a);
			float t = dot(w2, c);
			if (t < -epsilon) return false;
			float u = -dot(w2, b);
			if (u < -epsilon) return false;
			if (-s < t + u) return false;
		}
		else if (s < -epsilon) {
			glm::vec3 w2 = cross(d, a);
			float t = dot(w2, c);
			if (t > epsilon) return false;
			float u = -dot(w2, b);
			if (u > epsilon) return false;
			if (-s > t + u) return false;
		}
		else return false;
	}

	return true;
}

bool Octree::checkBoundaries(glm::vec3 pos, glm::vec3 P, glm::vec3 Q, glm::vec3 R) {
	glm::vec3 A = glm::vec3(1.0f, 0.0f, 1.0f) + pos;
	glm::vec3 B = glm::vec3(-1.0f, 0.0f, -1.0f) + pos;
	if (P.z <= A.z && P.z >= B.z && P.x <= A.x  && P.x >= B.x) return true;
	if (Q.z <= A.z && Q.z >= B.z && Q.x <= A.x  && Q.x >= B.x) return true;
	if (R.z <= A.z && R.z >= B.z && R.x <= A.x  && R.x >= B.x) return true;
	return false;
}

MeshStruct Octree::calculateVertices() {
	MeshStruct MS;
	this->calculateVerticesRecursive(MS);

	return MS;
}

void Octree::calculateVerticesRecursive(MeshStruct& MS) {
	for (GLuint i = 0; i < 4; i++) {
		Vertex v;
		v.Position.x = this->Bounds.position.x + this->Bounds.size * offsetsEdges[i].x;
		v.Position.y = this->Bounds.position.y + this->Bounds.size * offsetsEdges[i].y;
		v.Position.z = this->Bounds.position.z + this->Bounds.size * offsetsEdges[i].z;
		MS.Vertices.push_back(v);
		v.Position.x = this->Bounds.position.x + this->Bounds.size * offsetsEdges[(i + 1) % 4].x;
		v.Position.y = this->Bounds.position.y + this->Bounds.size * offsetsEdges[(i + 1) % 4].y;
		v.Position.z = this->Bounds.position.z + this->Bounds.size * offsetsEdges[(i + 1) % 4].z;
		MS.Vertices.push_back(v);
	}

	for (GLuint i = 0; i < 4; i++) {
		Vertex v;
		v.Position.x = this->Bounds.position.x + this->Bounds.size * offsetsEdges[i + 4].x;
		v.Position.y = this->Bounds.position.y + this->Bounds.size * offsetsEdges[i + 4].y;
		v.Position.z = this->Bounds.position.z + this->Bounds.size * offsetsEdges[i + 4].z;
		MS.Vertices.push_back(v);
		v.Position.x = this->Bounds.position.x + this->Bounds.size * offsetsEdges[((i + 1) % 4) + 4].x;
		v.Position.y = this->Bounds.position.y + this->Bounds.size * offsetsEdges[((i + 1) % 4) + 4].y;
		v.Position.z = this->Bounds.position.z + this->Bounds.size * offsetsEdges[((i + 1) % 4) + 4].z;
		MS.Vertices.push_back(v);
	}

	for (GLuint i = 0; i < 4; i++) {
		Vertex v;
		v.Position.x = this->Bounds.position.x + this->Bounds.size * offsetsEdges[i].x;
		v.Position.y = this->Bounds.position.y + this->Bounds.size * offsetsEdges[i].y;
		v.Position.z = this->Bounds.position.z + this->Bounds.size * offsetsEdges[i].z;
		MS.Vertices.push_back(v);
		v.Position.x = this->Bounds.position.x + this->Bounds.size * offsetsEdges[i + 4].x;
		v.Position.y = this->Bounds.position.y + this->Bounds.size * offsetsEdges[i + 4].y;
		v.Position.z = this->Bounds.position.z + this->Bounds.size * offsetsEdges[i + 4].z;
		MS.Vertices.push_back(v);
	}

	for (GLuint i = 0; i < 8; i++) {
		if (this->alreadyCreated[i]) this->children[i]->calculateVerticesRecursive(MS);
	}
}

Octree::~Octree() {
	for (GLuint i = 0; i < 8; i++) {
		if (this->alreadyCreated[i]) this->children[i].reset();
	}

	this->triangles.clear();
	std::vector<OctreeNodeTriangle>().swap(this->triangles);
	if (this->parent == NULL) {
		for (GLuint i = 0; i < this->verticesReferences->size(); i++) {
			this->verticesReferences->at(i).reset();
		}
		//this->verticesReferences->clear();
	}
	this->verticesReferences.reset();
	this->trianglesToMove.clear();
	std::vector<OctreeNodeTriangle>().swap(this->trianglesToMove);
	this->ModelsIdList.clear();
	std::map<GLuint, std::shared_ptr<ModelLoader>>().swap(this->ModelsIdList);
}

#pragma endregion

#pragma region OctreeNodeMesh

Octree::OctreeNodeTriangle::OctreeNodeTriangle(GLuint meshNumber, GLuint index1, GLuint index2, GLuint index3, std::shared_ptr<ModelLoader> ML, std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>> verticesRef) : ML(ML), verticesRef(verticesRef) {
	this->meshNumber = meshNumber;
	this->indices.push_back(index1);
	this->indices.push_back(index2);
	this->indices.push_back(index3);
}

Octree::OctreeNodeTriangle& Octree::OctreeNodeTriangle::operator= (const OctreeNodeTriangle &ref) {
	if (this != &ref) {
		this->ML.reset();
		this->verticesRef.reset();
		this->indices.clear();
		this->meshNumber = ref.meshNumber;
		this->ML = ref.ML;
		this->verticesRef = ref.verticesRef;
		this->indices.push_back(ref.indices[0]);
		this->indices.push_back(ref.indices[1]);
		this->indices.push_back(ref.indices[2]);
	}
	return *this;
}

Octree::OctreeNodeTriangle::~OctreeNodeTriangle() {
	this->indices.clear();
	std::vector<GLuint>().swap(this->indices);
	this->ML.reset();
	this->verticesRef.reset();
}
#pragma endregion