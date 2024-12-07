//#ifndef OCTREE_H
//#define OCTREE_H
//
//#pragma once
//#include "BoundingBox.h"
//#include "GameObject.h"
//#include <vector>
//#include <array>
//
//struct OctreeNode {
//
//	OctreeNode(const BoundingBox& bbox) {
//		this->bbox = bbox;
//	}
//
//
//	BoundingBox bbox;
//	std::vector<GameObject> contained_objects; // objects contained in the bbox
//	std::array<OctreeNode*, 8> children = { nullptr };
//
//	bool isLeaf() const {
//		return children[0] == nullptr;
//	}
//
//};
//
//class Octree
//{
//public:
//	Octree(const BoundingBox& bounds, int max_depth, int max_points_per_node) {
//		this->max_depth = max_depth;
//		this->max_points_per_node = max_points_per_node;
//		root = new OctreeNode(bounds);
//	}
//
//	~Octree() { delete root; }
//
//	/* Find gameObjects contained in specificed region */
//	std::vector<GameObject> query(const BoundingBox& region) const {
//		return query(root, region);
//	}
//
//	void DebugDraw() {
//
//
//
//	}
//
//private:
//
//	OctreeNode* root;
//
//	int max_depth;
//	int max_points_per_node;
//
//	void Subdivide(OctreeNode* node);
//	void Insert(OctreeNode* node, const GameObject& go, int depth);
//
//	std::vector<GameObject> query(OctreeNode* node, const BoundingBox& region) const {
//		std::vector<GameObject> result;
//
//		if (!node || !node->bbox.intersects(region)) return result;
//
//		for (const auto& obj : node->contained_objects) {
//			if (region.contains(obj)) result.push_back(obj);
//		}
//
//		// Recurse into children
//		for (auto* child : node->children) {
//			auto childResult = query(child, region);
//			result.insert(result.end(), childResult.begin(), childResult.end());
//		}
//
//		return result;
//	}
//
//
//};
//
//#endif