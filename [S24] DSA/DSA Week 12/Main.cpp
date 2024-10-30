#include <iostream>
#include <vector>
#include <algorithm>

namespace DisjointSet {
    class DisjointIntSet {
    public:
        virtual int find(int i) = 0;
        virtual void unionSet(int i, int j) = 0;
    };

    class DisjointIntSetImpl : public DisjointIntSet {
    private:
        std::vector<int> parent;
    public:
        DisjointIntSetImpl(int n) : parent(n + 1) {
            for (int i = 0; i <= n; i++) {
                parent[i] = i;
            }
        }

        int find(int i) override {
            if (parent[i] == i)
                return i;
            return parent[i] = find(parent[i]);
        }

        void unionSet(int i, int j) override {
            parent[find(i)] = find(j);
        }
    };
}

class Edge {
public:
    int src;
    int dist;
    int weight;

    Edge(int src, int dist, int weight) : src(src), dist(dist), weight(weight) {}

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class MyHashMap {
private:
    std::vector<Edge*> buckets;
    int numBuckets;

public:
    MyHashMap(int numBuckets) : numBuckets(numBuckets), buckets(numBuckets, nullptr) {}

    ~MyHashMap() {
        for (auto& bucket : buckets) {
            Edge* current = bucket;
            while (current != nullptr) {
                Edge* next = current->next;
                delete current;
                current = next;
            }
        }
    }

    void insert(int key, Edge* edge) {
        int index = key % numBuckets;
        Edge* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value.push_back(edge);
                return;
            }
            current = current->next;
        }
        buckets[index] = new Edge(key, std::vector<Edge*>{edge});
    }

    std::vector<Edge*> get(int key) {
        int index = key % numBuckets;
        Edge* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return std::vector<Edge*>();
    }
};

void quicksort(std::vector<Edge>& edges, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(edges, low, high);
        quicksort(edges, low, pivotIndex - 1);
        quicksort(edges, pivotIndex + 1, high);
    }
}

int partition(std::vector<Edge>& edges, int low, int high) {
    Edge pivot = edges[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (edges[j].weight <= pivot.weight) {
            i++;
            std::swap(edges[i], edges[j]);
        }
    }
    std::swap(edges[i + 1], edges[high]);
    return (i + 1);
}

void kruskalMST(std::vector<Edge>& edges, int n) {
    DisjointSet::DisjointIntSetImpl disjoint(n);
    quicksort(edges, 0, edges.size() - 1);

    std::vector<Edge> mst;
    MyHashMap trees(n + 1);
    std::vector<bool> vert(n + 1, false);

    for (const Edge& e : edges) {
        if (disjoint.find(e.src) != disjoint.find(e.dist)) {
            disjoint.unionSet(e.src, e.dist);
            mst.push_back(e);
        }
    }

    for (const Edge& e : mst) {
        int root = disjoint.find(e.src);
        std::vector<Edge*> treeEdges = trees.get(root);
        treeEdges.push_back(&e);
        trees.insert(root, &e);
    }

    std::vector<int> aloneVertices;
    for (int i = 1; i <= n; i++) {
        if (!vert[i]) {
            aloneVertices.push_back(i);
        }
    }

    std::cout << trees.size() + aloneVertices.size() << std::endl;
    for (const auto& t : trees) {
        std::cout << t.second.size() << " " << t.first << std::endl;
        for (Edge* e : t.second) {
            std::cout << e->src << " " << e->dist << " " << e->weight << std::endl;
        }
    }
    for (int v : aloneVertices) {
        std::cout << "1 " << v << std::endl;
    }
}

int main() {
    std::vector<Edge> edges;
    int n, k;
    std::cin >> n >> k;
    for (int i = 0; i < k; i++) {
        int src, dist, weight;
        std::cin >> src >> dist >> weight;
        edges.emplace_back(src, dist, weight);
    }
    kruskalMST(edges, n);
    return 0;
}
