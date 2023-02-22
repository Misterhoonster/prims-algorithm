#include "minheap.hpp"
using namespace std;

// Create a heap w/ size max_size
MinHeap::MinHeap(int cap)
{
	cur_size = 0;
	max_size = cap;
	heap = new pair<int, double>[cap];
}

// Inserts a new key
void MinHeap::insert(int v, double w)
{
    int i;

	if (cur_size == max_size)
	{
		cout << "\nOverflow error: min heap at max size\n";
		return;
	}

    // Insert new vertex
    if (vertex_to_index.find(v) == vertex_to_index.end())
    {
        // Add key to the end of the heap
        pair<int, double> new_val = make_pair(v, w);
        cur_size++;
        i = cur_size - 1;
        heap[i] = new_val;

        // add vertex to hash map
        vertex_to_index[v] = i;
    }
    // Update existing vertex
    else
    {
        // Find index of vertex and update its weight
        i = vertex_to_index[v];
        heap[i].second = w;
    }

    // Iteratively move the key up the tree until heap property is satisfied
    while (i != 0 && heap[parent(i)].second > heap[i].second)
    {
        swap(&heap[i], &heap[parent(i)], i, parent(i));
        i = parent(i);
    }
}

// Remove minimum element from min heap
int MinHeap::extract_min()
{
    // Handle base cases
	if (cur_size <= 0) {
		return INT_MAX;
    }

    vertex_to_index.erase(heap[0].first);

	if (cur_size == 1)
	{
		cur_size--;
		return heap[0].first;
	}


	// Store the minimum value, and remove it from heap
	int root = heap[0].first;
	heap[0] = heap[cur_size-1];
	cur_size--;
	min_heapify(0);

	return root;
}

// Rearranges the tree rooted at i to be a min-heap
// Assumes the subtrees already satisfy the heap property
void MinHeap::min_heapify(int i)
{
	int l = left(i);
	int r = right(i);
	int smallest = i;
	if (l < cur_size && heap[l].second < heap[i].second)
		smallest = l;
	if (r < cur_size && heap[r].second < heap[smallest].second)
		smallest = r;
	if (smallest != i)
	{
		swap(&heap[i], &heap[smallest], i, smallest);
		min_heapify(smallest);
	}
}

// A utility function to swap two elements
void MinHeap::swap(pair<int, double> *x, pair<int, double> *y, int x_i, int y_i)
{
    vertex_to_index[x->first] = y_i;
    vertex_to_index[y->first] = x_i;
	pair<int, double> temp = *x;
	*x = *y;
	*y = temp;
}

// Print tree
void MinHeap::print_tree() {
    for (int i = 0; i < cur_size; i++) {
        cout << heap[i].first << " " << heap[i].second << '\n';
    }
    for (auto const &pair: vertex_to_index) {
        cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

// Driver program to test above functions
// int main()
// {
// 	MinHeap h(11);
// 	h.insert(3, 0.4);
// 	h.insert(2, 0.2);
// 	h.insert(15, 1);
// 	h.insert(5, 1.4);
// 	h.insert(4, 0.33);
// 	h.insert(45, 0.6);
//     h.insert(3, 0.2);
//     h.insert(8, 0.7);
// 	cout << h.extract_min() << " ";
// 	cout << h.get_min() << " ";
// 	// h.decrease_key(45, 0);
// 	cout << h.get_min() << '\n';
//     h.print_tree();
// 	return 0;
// }
