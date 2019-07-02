#pragma once
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
		ModelLoader& ML;
		GLuint meshNumber;
		std::vector<GLuint> indices;
		std::reference_wrapper<std::vector<std::reference_wrapper<std::vector<Vertex>>>> verticesRef;
		OctreeNodeTriangle(GLuint meshNumber, GLuint index1, GLuint index2, GLuint index3, ModelLoader& ML, std::vector<std::reference_wrapper<std::vector<Vertex>>> verticesRef);
		OctreeNodeTriangle& operator= (const OctreeNodeTriangle &ref);
	};

	GLuint maxDepth;
	GLuint depth;
	OctreeBounds Bounds;
	Octree* parent;
	Octree* children[8];
	GLuint alreadyCreated[8];

	std::vector<std::reference_wrapper<std::vector<Vertex>>> vertices;
	std::vector<OctreeNodeTriangle> triangles;
	std::vector<OctreeNodeTriangle> trianglesToMove;
	std::vector<GLuint> ModelsIdList;

	bool insertRecursive(std::vector<VertexOctree>& vertices, std::vector<IndexOctree>& indices, GLuint depth, GLuint meshNumber, ModelLoader& model, std::vector<std::reference_wrapper<std::vector<Vertex>>> verticesRef);
	bool insertRecursive(OctreeNodeTriangle& triangle, GLuint depth);
	void updateOctree(GLuint id);
	bool checkBoundingBox(OctreeNodeTriangle& triangle);
	void moveTrianglesUp(OctreeNodeTriangle &triangle);
	void moveTriangleDown();

public:
	Octree() {};
	Octree(Octree *parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth);
	void addObject(ModelLoader& model);
};