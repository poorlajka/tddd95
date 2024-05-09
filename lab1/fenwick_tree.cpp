// Author: Viktor Bergström (vikbe588)
// Problem: Implementing a Fenwick Tree

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

// Computes the value of the least significant 1 in the binary representation of i
auto lsb(int i) -> int {
    return i & -i;
}

/*
    Simple implementation of a Fenwick Tree
    The tree supports two operations:
        1. Increment the value of an index by some value delta
        2. Query the sum of all values in the range 0..i for some index i
    Both these operations run in O(log(n))
    The tree cannot be instantiated from an existing array, instead you
    give it a size and get a tree where all indexes begin with a value of 0.

    The tree works by associating each index with another index based on the
    least significant 1 in its binary representation.

    When querying the sum of an index we simply sum the value of that index
    + the value at new_index=(index-lsb(index)) and then repeat this process until
    we reach the start/bottom of our array. In the worst case (large i all bits are 1), 
    the nr of repetitions will equal the nr of bit's and thus it's obvious to see that
    this then is in O(log(n)).

    When incrementing an index we do the same procedure but in reverse.
    We increment the value at the index and then increment the value at index+lsb(index) and so on...

    Note (array is 1 indexed)
*/ 
struct FenwickTree {

    vector<long> tree_array;

    FenwickTree(int array_len) {
        tree_array = vector<long>(array_len+1, 0);
    }

    // Compute the sum of the first numbers up to, but not including a[index]
    auto prefix_sum_query(int index) -> long {
        if (index == 0) { return 0; }; // index=0 is interpreted as an empty sum

        long query_result = 0;
        while (index > 0) {
            query_result += tree_array[index];
            index -= lsb(index);
        }
        return query_result;
    }

    auto increment_index(int index, long delta) -> void {
        index++; //Array is 1 indexed
        while (index < tree_array.size()) {
            tree_array[index] += delta;
            index += lsb(index);
        }
    }
};

auto main() -> int {
    int array_len, nr_of_operations;
    cin >> array_len >> nr_of_operations;

    stringstream query_results;
    FenwickTree fenwick_tree(array_len);
    for (int i = 0 ; i < nr_of_operations; ++i) {
        char op;
        int index;
        long delta;
        cin >> op >> index;
        if (op == '+') {
            cin >> delta;
            fenwick_tree.increment_index(index, delta);
        }
        if (op == '?') {
            query_results << fenwick_tree.prefix_sum_query(index) << "\n";
        }
    }
    cout << query_results.str();
    return 0;
}