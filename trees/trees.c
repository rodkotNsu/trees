#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include <stdio.h>

#define SIZE(x) (sizeof(x) / sizeof(x[0]))


struct Tree {
	char value;
	struct Tree* left;
	struct Tree* rigth;
};
  typedef struct Tree Tree;

size_t maxx(size_t value0, size_t value1) {
	if (value0 > value1)
		return value0;
	else
		return value1;
}


size_t tree_height(struct Tree* tree) {
	if (tree == NULL)
		return 0;
	return maxx(tree_height(tree->left), tree_height(tree->rigth)) + 1;
}


void tree_add(struct Tree** root, int data) {
	if (*root == NULL) {
		Tree* roots = (Tree*)malloc(sizeof(Tree));

		(*roots).value = data;
		roots->left = NULL;
		roots->rigth = NULL;

		*root = roots;
	}
	else {
		if ((*root)->value > data) 
			tree_add(&((*root)->left), data);
		else 
			tree_add(&((*root)->rigth), data);
	}
}


struct Tree* tree_find(struct Tree* root, int data) {
	if (root == NULL || root->value == data)
		return root;
	
	if (root->value > data) 
		return tree_find(root->left, data);
	else
		return tree_find(root->rigth, data);
}


size_t tree_count_nodes(struct Tree* root) {
	if (root == NULL) 
		return 0;
	return 1 + tree_count_nodes(root->left) + tree_count_nodes(root->rigth);
}


void tree_destroy(struct Tree* root) {
	if (root == NULL) 
		return;
	
	tree_destroy(root->left);
	tree_destroy(root->rigth);
	
	free(root);
}

int main() {
	// ”становить автоматическое определение утечек пам€ти
	// «апустите в режиме Start Debugging (через F5) чтобы проверить наличие утечек пам€ти (Visual Studio выведет результат в окно Output)
	// —м. https://docs.microsoft.com/ru-ru/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2015&redirectedfrom=MSDN
	// ѕользователи других компил€торов должны воспользоватс€ утилитой valgrind						
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		printf("Testing addition - ");
		struct Tree* tree = NULL;
		tree_add(&tree, 5);
		tree_add(&tree, 7);
		assert(tree != NULL);

		tree_destroy(tree);
		printf("\t Done\n\n");
	}

	{
		printf("Testing empty tree - ");
		struct Tree* empty_tree = NULL;

		assert(tree_height(empty_tree) == 0);
		assert(tree_count_nodes(empty_tree) == 0);
		assert(tree_find(empty_tree, 5) == NULL);

		tree_destroy(empty_tree);
		printf("\t Done\n\n");
	}

	{
		printf("Testing single node tree - ");
		int data = 5;
		struct Tree* single_node_tree = NULL;
		tree_add(&single_node_tree, data);

		assert(tree_height(single_node_tree) == 1);
		assert(tree_count_nodes(single_node_tree) == 1);
		assert(tree_find(single_node_tree, data) == single_node_tree);
		assert(tree_find(single_node_tree, 42) == NULL);

		tree_destroy(single_node_tree);
		printf("\t Done\n\n");
	}

	{
		printf("Testing tree resembling a list - ");
		int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		struct Tree* degenerate_tree = NULL;
		for (size_t i = 0; i < SIZE(data); i++) {
			tree_add(&degenerate_tree, data[i]);
		}

		assert(tree_height(degenerate_tree) == 10);
		assert(tree_count_nodes(degenerate_tree) == 10);
		assert(tree_find(degenerate_tree, 5) != NULL);
		assert(tree_find(degenerate_tree, 42) == NULL);

		tree_destroy(degenerate_tree);
		printf("\t Done\n\n");
	}

	{
		printf("Testing balanced tree - ");
		int data[] = { 16, 8, 4, 12, 24, 20, 28 };
		struct Tree* balanced_tree = NULL;
		for (size_t i = 0; i < SIZE(data); i++) {
			tree_add(&balanced_tree, data[i]);
		}

		assert(tree_height(balanced_tree) == 3);
		assert(tree_count_nodes(balanced_tree) == 7);
		assert(tree_find(balanced_tree, 28) != NULL);
		assert(tree_find(balanced_tree, 42) == NULL);

		tree_destroy(balanced_tree);
		printf("\t Done\n\n");
	}

}