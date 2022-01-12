#include "binary_tree.h"
#include <cassert>
#include <stdexcept>
#include <iostream>

template <typename key_t, typename value_t>
const typename binary_tree<key_t, value_t>::node *binary_tree<key_t, value_t>::get_root() const {
	return root;
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::node::node(key_t const &key, value_t const &value)
	: data(key, value), left(nullptr), right(nullptr), parent(nullptr) {
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::binary_tree()
	: root(nullptr) {
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::binary_tree(node *root)
	: root(root) {
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::binary_tree(const binary_tree &&tree)
	: root(tree.root) {
	tree.root = nullptr;
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::~binary_tree() {
	while (root) {
		erase(begin());
	}
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::iterator::iterator(node *ptr, binary_tree const *tree)
	: ptr(ptr), tree(tree) {
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t>::iterator::iterator()
	: ptr(nullptr), tree(nullptr) {
}

template <typename key_t, typename value_t>
bool binary_tree<key_t, value_t>::iterator::operator!=(const iterator &it) {
	return (tree != it.tree) || (ptr != it.ptr);
}

template <typename key_t, typename value_t>
bool binary_tree<key_t, value_t>::iterator::operator==(const iterator &it) {
	return (tree == it.tree) && (ptr == it.ptr);
}

template <typename key_t, typename value_t>
std::pair<const key_t, value_t> &binary_tree<key_t, value_t>::iterator::operator*() {
	return ptr->data;
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator &binary_tree<key_t, value_t>::iterator::operator++() {
	if (ptr == nullptr) {
		throw std::runtime_error("Increment of end iterator.");
	}
	tree->splay(ptr);

	if (ptr->right == nullptr) {
		ptr = nullptr;
		return *this;
	}

	ptr = ptr->right;

	while (ptr->left != nullptr) {
		ptr = ptr->left;
	}
	tree->splay(ptr);
	return *this;
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator binary_tree<key_t, value_t>::iterator::operator++(int) {
	iterator res(ptr, tree);
	++(*this);
	return res;
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator &binary_tree<key_t, value_t>::iterator::operator--() {
	if (ptr == nullptr) {
		throw std::runtime_error("Decrement of begin iterator.");
	}
	tree->splay(ptr);

	if (ptr->left == nullptr) {
		ptr = nullptr;
		return *this;
	}

	ptr = ptr->left;

	while (ptr->right != nullptr) {
		ptr = ptr->right;
	}
	tree->splay(ptr);
	return *this;
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator binary_tree<key_t, value_t>::iterator::operator--(int) {
	iterator res(ptr, tree);
	--(*this);
	return res;
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator binary_tree<key_t, value_t>::insert(key_t const &key, value_t const &value) {
	binary_tree<key_t, value_t> left_tree(split(key));
	node *res;

	if ((root != nullptr) && (root->data.first == key)) {
		root->data.second = value;
		res = root;
		merge(std::move(left_tree));
	}
	else {
		res = new node(key, value);
		res->left = left_tree.root;
		if (left_tree.root != nullptr) {
			left_tree.root->parent = res;
			left_tree.root = nullptr;
		}

		res->right = root;
		if (root != nullptr) {
			root->parent = res;
		}
		root = res;
	}
	return iterator(res, this);
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator binary_tree<key_t, value_t>::find(key_t const &key) {
	if (root == nullptr) {
		return iterator(nullptr, this);
	}

	node *p = root;
	while (p->data.first != key) {
		if (p->data.first < key) {
			if (p->right == nullptr) {
				splay(p);
				return iterator(nullptr, this);
			}
			p = p->right;
		}
		else {
			if (p->left == nullptr) {
				splay(p);
				return iterator(nullptr, this);
			}
			p = p->left;
		}
	}

	splay(p);
	return iterator(p, this);
}

template <typename key_t, typename value_t>
void binary_tree<key_t, value_t>::erase(iterator it) {
	splay(it.ptr);

	binary_tree<key_t, value_t> left_tree(it.ptr->left);

	if (left_tree.root != nullptr) {
		left_tree.root->parent = nullptr;
	}

	root = it.ptr->right;
	if (root != nullptr) {
		root->parent = nullptr;
	}

	merge(std::move(left_tree));

	delete it.ptr;
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator binary_tree<key_t, value_t>::begin() const {
	if (root == nullptr) {
		return iterator(nullptr, this);
	}
	while (root->left != nullptr) {
		root = root->left;
	}
	splay(root);
	return iterator(root, this);
}

template <typename key_t, typename value_t>
typename binary_tree<key_t, value_t>::iterator binary_tree<key_t, value_t>::end() const {
	return iterator(nullptr, this);
}

template <typename key_t, typename value_t>
binary_tree<key_t, value_t> binary_tree<key_t, value_t>::split(key_t const &key) {
	if (root == nullptr) {
		return binary_tree<key_t, value_t>();
	}

	node *nearest_from_right = nullptr;

	while (root->data.first != key) {
		if (root->data.first < key) {
			if (root->right == nullptr) {
				break;
			}
			root = root->right;
		}
		else {
			nearest_from_right = root;
			if (root->left == nullptr) {
				break;
			}
			root = root->left;
		}
	}

	splay(root);

	if (root->data.first == key) {
		nearest_from_right = root;
	}

	if (nearest_from_right == nullptr) {
		node *res = root;
		root = nullptr;
		return binary_tree<key_t, value_t>(res);
	}

	splay(nearest_from_right);

	node *res = root->left;
	root->left = nullptr;
	if (res != nullptr) {
		res->parent = nullptr;
	}
	return binary_tree<key_t, value_t>(res);
}

template <typename key_t, typename value_t>
void binary_tree<key_t, value_t>::merge(binary_tree &&tree) {
	if (root == nullptr) {
		root = tree.root;
		tree.root = nullptr;
		return;
	}
	while (root->left != nullptr) {
		root = root->left;
	}
	splay(root);

	root->left = tree.root;
	if (tree.root != nullptr) {
		tree.root->parent = root;
		tree.root = nullptr;
	}
}

template <typename key_t, typename value_t>
void binary_tree<key_t, value_t>::splay(node *ptr) const {
	while (ptr->parent != nullptr) {
		if (ptr->parent->parent == nullptr) {
			ptr->rotate_with_parent();
		}
		else {
			if (((ptr->parent->left == ptr) && (ptr->parent->parent->left == ptr->parent)) ||
				((ptr->parent->right == ptr) && (ptr->parent->parent->right == ptr->parent))) { // zig-zig case
				ptr->parent->rotate_with_parent();
				ptr->rotate_with_parent();
			}
			else { // zig-zag case
				ptr->rotate_with_parent();
				ptr->rotate_with_parent();
			}
		}
	}
	root = ptr;
}

template <typename key_t, typename value_t>
void binary_tree<key_t, value_t>::node::rotate_with_parent() {
	assert(parent != nullptr);
	node *parent_of_parent = parent->parent;

	if (parent->left == this) {
		parent->left = right;
		if (right != nullptr) {
			right->parent = parent;
		}

		right = parent;
	}
	else {
		parent->right = left;
		if (left != nullptr) {
			left->parent = parent;
		}

		left = parent;
	}

	parent->parent = this;

	if (parent_of_parent != nullptr) {
		if (parent_of_parent->left == parent) {
			parent_of_parent->left = this;
		}
		else {
			parent_of_parent->right = this;
		}
	}
	parent = parent_of_parent;
}
