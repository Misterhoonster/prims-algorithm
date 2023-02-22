#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <cmath>
#include <set>
#include <iterator>
#include "minheap.hpp"

using namespace std;
using namespace std::chrono;



class Graph {
private:
    int n;
    int d;
    vector<pair<int, double>> *adj;

public:
    Graph(int n, int d) {
        this->n = n;
        this->d = d;
        adj = new vector<pair<int, double>>[n];
    }

    ~Graph() {
        delete[] adj;
    }

    void addEdge(int u, int v, double weight){
        adj[u].push_back(make_pair(v, weight));
        adj[v].push_back(make_pair(u, weight));
    }

    // Generates a random double between 0.0 and 1.0
    double rand_d(){
        return static_cast<double>(rand())/RAND_MAX;
    }

    // Randomly generate a d-dimensional coordinate
    double* generate_coord(int d) {
        double* array = new double[d];
        for (int i = 0; i < d; i++) {
            array[i] = rand_d();
        }
        return array;
    }

    // Finds distance between two d-dimensional coordinates
    double dist(double* x, double* y, int d) {
        double distance = 0.0;
        for (int i = 0; i < d; i++) {
            distance += (x[i] - y[i]) * (x[i] - y[i]);
        }
        return sqrt(distance);
    }

    double prune(int d){
        switch (d)
        {
            case 0: return 15.0/n;
            case 2: return 3.0/pow(n, 0.5);
            case 3: return 2.0/pow(n, 0.33);
            case 4: return 2.0/pow(n, 0.25);
        }
        return 2/pow(n, 1/4);
    }

    void compute_edges() {
        // Determine edge weight cutoff
        double cutoff = this->prune(d);
        
        // Generate 0-dimensional edge weights
        if (d == 0)
        {
            for (int i = 0; i < n; i++) {
                for (int j = i+1; j < n; j++) {
                    double dist = this->rand_d();
                    // cout << "(" << i << ", " << j << ") " << dist << "\n";
                    if (dist < cutoff) {
                        this->addEdge(i, j, dist);
                    }
                }
            }
            return;
        }
        // Generate n d-dimensional coords
        double** coords = new double*[n];
        for (int i = 0; i < n; i++) {
            coords[i] = generate_coord(d);
        }
        
        // Add all weighted edges to the graph
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                double dist = this->dist(coords[i], coords[j], d);
                if (dist < cutoff){
                    this->addEdge(i, j, dist);
                }
            }
        }
        delete[] coords;
    }

    // Run Prim's algorithm - find the average weight of edges in MST
    double run_prims()
    {
        // Initialize data structures
        double* dist = new double[n];
        int* prev = new int[n];
        set<int> S;
        MinHeap heap(n);

        // Insert first vertex into heap
        heap.insert(0, 0);

        // Set distance values
        for (int i = 0; i < n; i++) {
            dist[i] = INT_MAX;
        }
        dist[0] = 0;

        // Build MST
        while (heap.get_size() > 0)
        {
            int v = heap.extract_min();
            S.insert(v);
            vector<pair<int, double>> edges = adj[v];
            for (const auto& pair : edges)
            {
                int w = pair.first;
                double weight = pair.second;
                if (S.find(w) == S.end() && dist[w] > weight)
                {
                    dist[w] = weight;
                    prev[w] = v;
                    heap.insert(w, weight);
                }
            }
        }

        // Calculate average weight of edges in MST
        double total_weight = 0;
        // double max_edge = 0;
        for (int v : S)
        {
            if (v != 0)
            {
                int u = prev[v];
                auto it = find_if(adj[u].begin(), adj[u].end(), [v](pair<int, double>& e){return e.first == v;});
                // cout << "(" << u << ", " << it->first << ") " << it->second << "\n";
                total_weight += it->second;
                // if (it->second > max_edge)
                // {
                //     max_edge = it->second;
                // }
            }
        }
        return total_weight;
        // return max_edge;
    }

    // Print adjacency list representation of graph
    void print_graph()
    {
        int v;
        double w;
        for (int u = 0; u < n; u++)
        {
            cout << "Node " << u << " makes an edge with \n";
            for (auto it = adj[u].begin(); it!=adj[u].end(); it++)
            {
                v = it->first;
                w = it->second;
                cout << "\tNode " << v << " with edge weight ="
                    << w << "\n";
            }
            cout << "\n";
        }
    }
};

int main(int argc, char* argv[]) {
    // no args
    if (argc == 1)
    {
        cout << "Input error: invalid number of arguments";
        return 0;
    }

    // first arg set to 1
    if (atoi(argv[1]) == 1)
    {
        int trials = 5;
        int dim[] = {3};
        int N[] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144};
        for (int d : dim)
        {
            for (int n : N)
            {
                double avg_weight = 0;
                for (int i = 0; i < trials; i++)
                {
                    Graph g(n, d);
                    g.compute_edges();
                    avg_weight += g.run_prims();
                    // max_edge = cur_edge > max_edge ? cur_edge : max_edge;
                }
                avg_weight /= trials;
                cout << "Avg. weight for (n = " << n << ", d=" << d << "): "  << avg_weight << "\n";
            }
            cout << "\n";
        }
        return 0;
    }

    // invalid number of args
    if (argc != 5)
    {
        cout << "Input error: invalid number of arguments";
        return 0;
    }

    // proper input given
    int n = atoi(argv[2]);
    int trials = atoi(argv[3]);
    int d = atoi(argv[4]);

    double avg_weight = 0;
    for (int i = 0; i < trials; i++)
    {
        Graph g(n, d);
        g.compute_edges();
        avg_weight += g.run_prims();
    }
    avg_weight /= trials;
    cout << avg_weight << " " << n << " "  << trials << " " << d;

    return 0;
}

