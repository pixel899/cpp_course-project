#pragma once
#include <utility>

template <typename key_t, typename value_t>
class binary_tree {
public:
	struct node {
		std::pair<const key_t, value_t> data;

		node *left;
		node *right;
		node *parent;

		node(key_t const &key, value_t const &value);

		void rotate_with_parent();
	};

private:
	node mutable *root;

	// Разделяет дерево "tree" на два дерева :
	//   - в исходном дереве остаются эламенты >= "key"
	//   - возвращает дерево с элементами < "key"
	binary_tree<key_t, value_t> split(key_t const &key);

	// Объединяет текущее дерево с деревом "tree",
	// предполагается, что все ключи из "tree" меньше ключей из текущего дерева.
	void merge(binary_tree &&tree);

	// Выполняет операцию splay с одной из вершин дерева,
	// перемещая его в корень.
	void splay(node *ptr) const;

	binary_tree(node *root);
public:
	class iterator {
	private:
		node *ptr;
		binary_tree<key_t, value_t> const *tree;

		iterator(node *ptr, binary_tree const *tree);
	public:
		iterator();

		bool operator!=(const iterator &it);
		bool operator==(const iterator &it);

		std::pair<const key_t, value_t> &operator*();

		iterator &operator++();
		iterator operator++(int);

		iterator &operator--();
		iterator operator--(int);

		friend binary_tree;
	};

	binary_tree();
	binary_tree(const binary_tree &&tree);
	binary_tree(const binary_tree &) = delete;
	void operator=(const binary_tree &) = delete;

	~binary_tree();

	iterator insert(key_t const &key, value_t const &value);

	iterator find(key_t const &key);

	void erase(iterator);

	iterator begin() const;
	iterator end() const;

	const node *get_root() const;

	
};


