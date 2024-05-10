// Author: Viktor Bergström (vikbe588)
// Problem: 0/1 knapsack

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/* 
    Solving 0/1 knapsack using dynamic programming using bottom up approach 
*/
auto solve_knapsack(int n, int c, 
    vector<pair<int, int>> &objects) -> vector<int> {

    /* 
        Compute a n*c sized table containing the solutions for all the
        subproblems (and the main problem) in O(n*c)
    */
    vector<vector<int>> knapsack_values(n+1, vector<int>(c+1, 0));
    for (int n_i = 1; n_i < n+1; ++n_i) {
        auto [value, weight] = objects[n_i-1];

        for (int c_i = 1; c_i < c+1; ++c_i) {

            int new_value = knapsack_values[n_i-1][c_i-weight] + value;
            // Include the object if it leads to a greater value
            if (c_i >= weight && new_value > knapsack_values[n_i-1][c_i]) {
                knapsack_values[n_i][c_i] = new_value;
            }
            // Otherwise we exclude it
            else {
                knapsack_values[n_i][c_i] = knapsack_values[n_i-1][c_i];
            }
        }
    }

    /* 
        Backtrack using the previously computed table to determine the object selected.
    */
    int c_i = c;
    vector<int> indicies;
    for (int n_i = n; n_i > 0; --n_i) {
        /*
            If the current value is not the same as the one for the previous object then
            that means we must have not excluded it when computing the table
        */         
       if (knapsack_values[n_i][c_i] != knapsack_values[n_i-1][c_i]) {
            indicies.push_back(n_i-1);
            c_i -= objects[n_i-1].second;
        }
    }
    
    reverse(indicies.begin(), indicies.end());
    return indicies;
}

auto main () -> int {
    int c, n;
    while (cin >> c >> n) {
        vector<pair<int, int>> objects(n);
        for (int i = 0; i < n; ++i) {
            int value, weight;
            cin >> value >> weight;
            objects[i] = make_pair(value, weight);
        }

        vector<int> indicies = solve_knapsack(n, c, objects);
        cout << indicies.size() << "\n";
        for (int i = 0; i < indicies.size(); ++i) {
            cout << indicies[i];
            if (i < indicies.size() - 1)
                cout << " ";
        }
        cout << "\n";
    }
    return 0;
}