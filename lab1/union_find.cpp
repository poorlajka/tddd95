// Author: Viktor Bergström
// Problem: Union-Find/Disjoint-set

#include <iostream>
#include <vector>
#include <strstream>

using namespace std;

/*
    Simple implementation of the Disjoint-set data structure (aka Union-find) 
    which stores a collection of disjoint sets containing in this case integers.
    The Disjoint-set has two main operations: 
        1. unify(a, b) - merge the subset containing a and the subset containing b into one
        2. find(a) - find the root of the set which contains a 
    Both run in O(log(n)) (space complexity of O(n)) without path compression and rank
    With rank and path compression they run with amortized constant time complexity.
*/
struct DisjointSet {
    vector<int> parents;
    vector<int> ranks;

    DisjointSet(int len) {
        parents = vector<int>(len);
        ranks = vector<int>(len);

        // Initially every element has it's own set and is therefore it's own parent
        for (int i = 0; i < len; ++i) {
            parents[i] = i;
        }
    }

    auto find(int n) -> int {
        /*
            Iterate through parents until we find the root of the set
            which will be the node which is its own parent
        */
       int root = n;
        while (root != parents[root])
            root = parents[root];


        /*
            Perform path compression
            Starting from n and moving upward through the parent tree
            reassign the parent of each node to be the root.
            This flattens/compresses the tree which grants amortized constant time complexity.
        */
        while (n != root) {
            int temp = parents[n];
            parents[n] = root;
            n = temp;
        }

        return n;
    }

    auto unify(int u, int v) -> void {
        int root_u = find(u);
        int root_v = find(v);

        // If a and b are already in the same set we do't have to do anything
        if (root_u == root_v) {
            return;
        }

        /*
            Use ranks to ensure that the smaller set(tree) will be attached to the root of the larger one.
            This prevents the tree from becoming overly imbalanced.
        */
        if (ranks[root_u] < ranks[root_v]) {
            parents[root_u] = root_v;
        }
        else if (ranks[root_u] > ranks[root_v]) {
            parents[root_v] = root_u;
        }
        else {
            parents[root_v] = root_u;
            ranks[root_u]++;
        }
    }
};

auto main () -> int {
    int nr_of_elements, nr_of_operations;
    cin >> nr_of_elements >> nr_of_operations;

    DisjointSet ds(nr_of_elements);
    strstream answers;
    for (int i = 0; i < nr_of_operations; ++i) {
        char op;
        int a, b;
        cin >> op >> a >> b;

        // "= a b" indicate that the sets containing a and b are joined
        if (op == '=') {
            ds.unify(a, b);
        }
        // "? a b" is a query asking whether a and b belong to the same set
        else if (op == '?') {
            if (ds.find(a) == ds.find(b)) {
                answers << "yes\n";
            }
            else {
                answers << "no\n";
            }
        }
    }
    cout << answers.str();
    return 0;
}