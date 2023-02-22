#include<iostream>
#include <unordered_map>
#include<climits>
using namespace std;

class MinHeap
{
	pair<int, double> *heap; // pointer to array of elements in heap
	int max_size; // maximum possible size of min heap
	int cur_size; // Current number of elements in min heap
    unordered_map<int, int> vertex_to_index; // hash table mapping vertices to indices

public:
	// constructor
	MinHeap(int max_size);

	// rearrange the tree rooted at i to be a min heap
	void min_heapify(int i);

    // get index of i's parent
	int parent(int i) { return (i-1)/2; }

	// get index of i's left child
	int left(int i) { return (2*i + 1); }

	// get index of i's right child
	int right(int i) { return (2*i + 2); }

	// remove the smallest elt and rearrange so heap structure is maintained
	int extract_min();

	// return root key (smallest node) from min heap
	int get_min() { return heap[0].first; }

	// add the value (v, w) to the heap
	void insert(int v, double w);

    // swap nodes in tree
    void swap(pair<int, double> *x, pair<int, double> *y, int x_i, int y_i);

    // getter for size
    int get_size() { return cur_size; }
    
    // print tree
    void print_tree();
};