#pragma once
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>7
#include <glm/gtx/intersect.hpp>
#include "../../Strutture/MeshStruct.h"
#include "../../Model/ModelLoader.h"

const OctreeOffset offsets[8] = {
	{-0.5, -0.5, -0.5}, //bottom-left-rear
	{+0.5, -0.5, -0.5}, //bottom-right-rear
	{-0.5, +0.5, -0.5}, //bottom-left-front
	{+0.5, +0.5, -0.5}, //bottom-right-front
	{-0.5, -0.5, +0.5}, //up-left-rear
	{+0.5, -0.5, +0.5}, //up-right-rear
	{-0.5, +0.5, +0.5}, //up-left-front
	{+0.5, +0.5, +0.5} //up-right-front
};

class Octree {
private:

	class OctreeNodeTriangle {

	public:
		std::shared_ptr<ModelLoader> ML;
		GLuint meshNumber;
		std::vector<GLuint> indices;
		std::vector<std::shared_ptr<std::vector<Vertex>>> verticesRef;
		OctreeNodeTriangle(GLuint meshNumber, GLuint index1, GLuint index2, GLuint index3, std::shared_ptr<ModelLoader> ML, std::vector<std::shared_ptr<std::vector<Vertex>>> verticesRef);
		OctreeNodeTriangle& operator= (const OctreeNodeTriangle &ref);
	};

	GLuint maxDepth;
	GLuint depth;
	OctreeBounds Bounds;
	//std::weak_ptr<Octree> parent;
	Octree* parent;

	std::shared_ptr<Octree> children[8];
	GLuint alreadyCreated[8];

	std::vector<std::shared_ptr<std::vector<Vertex>>> vertices;
	std::vector<OctreeNodeTriangle> triangles;
	std::vector<OctreeNodeTriangle> trianglesToMove;
	std::map<GLuint, std::shared_ptr<ModelLoader>> ModelsIdList;

	bool insertRecursive(std::vector<VertexOctree>& vertices, std::vector<IndexOctree>& indices, GLuint depth, GLuint meshNumber, std::shared_ptr<ModelLoader> model, std::vector<std::shared_ptr<std::vector<Vertex>>> verticesRef);
	bool insertRecursive(OctreeNodeTriangle& triangle, GLuint depth);
	bool checkBoundingBox(OctreeNodeTriangle& triangle);
	void moveTrianglesUp(OctreeNodeTriangle &triangle);
	void moveTriangleDown();
	void checkCollisionRecursive(GLuint id, glm::vec3 translation);
	void checkCollisionRecursive(GLuint id, glm::vec3 translation, OctreeNodeTriangle& trianglesToCheck);

public:
	Octree() {};
	Octree(std::shared_ptr<Octree> parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth);
	Octree(Octree* parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth);
	void addObject(std::shared_ptr<ModelLoader> model);
	void updateOctree(GLuint id);
	inline std::shared_ptr<ModelLoader> getML(GLuint id) { return this->ModelsIdList.at(id); }
	inline OctreeBounds& getBounds() { return this->Bounds; }
};