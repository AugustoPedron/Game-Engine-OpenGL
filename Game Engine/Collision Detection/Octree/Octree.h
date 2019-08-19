#pragma once
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "../../Strutture/MeshStruct.h"
#include "../../Strutture/OctreeStruct.h"
#include "../../Model/ModelLoader.h"


const OctreeOffset offsets[8] = {
	{-0.25, -0.25, -0.25}, //bottom-left-rear
	{+0.25, -0.25, -0.25}, //bottom-right-rear
	{-0.25, +0.25, -0.25}, //up-left-rear
	{+0.25, +0.25, -0.25}, //up-right-rear
	{-0.25, -0.25, +0.25}, //bottom-left-front
	{+0.25, -0.25, +0.25}, //bottom-right-front
	{-0.25, +0.25, +0.25}, //up-left-front
	{+0.25, +0.25, +0.25}  //up-right-front
};

const OctreeOffset offsetsEdges[8] = {
	{-0.5, -0.5, -0.5}, //bottom-left-rear
	{+0.5, -0.5, -0.5}, //bottom-right-rear
	{+0.5, -0.5, +0.5}, //bottom-right-front
	{-0.5, -0.5, +0.5}, //bottom-left-front
	{-0.5, +0.5, -0.5}, //up-left-rear
	{+0.5, +0.5, -0.5}, //up-right-rear
	{+0.5, +0.5, +0.5}, //up-right-front
	{-0.5, +0.5, +0.5}  //up-left-front
};

struct inequality_coeff {
	float a;
	float b;
	float m;
	float n;
};

class Octree {
private:

	class OctreeNodeTriangle {

	public:
		std::shared_ptr<ModelLoader> ML;
		GLuint meshNumber;
		std::vector<GLuint> indices;
		std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>> verticesRef;
		OctreeNodeTriangle(GLuint meshNumber, GLuint index1, GLuint index2, GLuint index3, std::shared_ptr<ModelLoader> ML, std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>> verticesRef);
		OctreeNodeTriangle& operator= (const OctreeNodeTriangle &ref);
		~OctreeNodeTriangle();
	};

	GLuint maxDepth;
	GLuint depth;
	OctreeBounds Bounds;
	Octree* parent;

	std::shared_ptr<Octree> children[8];
	GLuint alreadyCreated[8];

	std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>> verticesReferences;
	std::vector<OctreeNodeTriangle> triangles;
	std::vector<OctreeNodeTriangle> trianglesToMove;
	std::map<GLuint, std::shared_ptr<ModelLoader>> ModelsIdList;
	std::vector<OctreeStruct> ModelsCounter;

	void insertRecursive(std::vector<IndexOctree>& indices, GLuint depth, GLuint meshNumber, std::shared_ptr<ModelLoader> model, std::shared_ptr<std::vector<std::shared_ptr<std::vector<Vertex>>>> verticesRef);
	bool insertRecursive(OctreeNodeTriangle& triangle, GLuint depth);
	void insertRecursive(OctreeNodeTriangle& triangle, GLuint depth, GLuint child);
	bool checkBoundingBox(OctreeNodeTriangle& triangle);
	bool checkParentBundingBox(OctreeNodeTriangle& triangle);
	GLuint checkChildrenBoundingBox(OctreeNodeTriangle& triangle);
	void moveTrianglesUp(OctreeNodeTriangle &triangle);
	bool checkCollisionRecursive(GLuint id, OctreeNodeTriangle& trianglesToCheck, glm::vec3& direction);
	bool checkSecondCollisionRecursive(GLuint id, OctreeNodeTriangle& trianglesToCheck, OctreeNodeTriangle& firstTriangle, glm::vec3& direction);
	bool triangleTriangleIntersection(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P, glm::vec3 Q, glm::vec3 R);
	bool triangleSegmentIntersection(glm::vec3 A, glm::vec3 B, glm::vec3 P, glm::vec3 Q, glm::vec3 R);
	bool checkBoundaries(glm::vec3 pos, glm::vec3 P, glm::vec3 Q, glm::vec3 R);
	void calculateVerticesRecursive(MeshStruct& MS);  
		
public:
	Octree() {};
	Octree(std::shared_ptr<Octree> parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth);
	Octree(Octree* parent, OctreeBounds Bounds, GLuint maxDepth, GLuint depth);
	~Octree();
	void addObject(std::shared_ptr<ModelLoader> model);
	void updateOctree(GLuint id);
	inline std::shared_ptr<ModelLoader> getML(GLuint id) { return this->ModelsIdList.at(id); }
	inline OctreeBounds& getBounds() { return this->Bounds; }
	MeshStruct calculateVertices();
	bool checkCollisionRecursive(GLuint id, glm::vec3& direction);
};