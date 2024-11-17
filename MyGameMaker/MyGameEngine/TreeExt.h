#pragma once

#include <list>
#include <vector>
#include <utility>
#include "readOnlyView.h"

template <class T>
class TreeExt {

protected:
	T* _parent = nullptr;
	std::vector<T> _children;

public:
	auto& parent() const { return *_parent; }
	auto children() const { return readOnlyVector<T>(_children); }

	auto& root() const { return _parent ? _parent->root() : *this; }
	bool isRoot() const { return !_parent; }

	template <typename ...Args>
	auto& emplaceChild(Args&&... args) {
		_children.emplace_back(std::forward<Args>(args)...);
		_children.back()._parent = static_cast<T*>(this);
		return _children.back();
	}

	//void removeChild(const T& child) { return _children.remove(std::forward(child)); }


	void removeChild(const T& child) {

		for (auto it = _children.begin(); it != _children.end(); ) {
			if ((*it) == child) {

				it = _children.erase(it); // Erase returns the next iterator.
				return; // Exit after removing the object.
			}
			else {
				++it; // Move to the next element if not removed.
			}
		}
	}

};