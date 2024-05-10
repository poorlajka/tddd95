// Author: Viktor Bergström (vikbe588)
// Interval cover

#include <iostream>
#include <vector>

#define NOT_FOUND -1

using namespace std;

/*
    Algorithm:
    1. let n = a (beginning of interval), 
        S = {} (set containing indexes of intervals for solution)

    2. Let M be the set of all intervals [a_i, b_i] where a_i <= n
    3. If M is empty then output "impossible" and stop 
    4. Find [a_j, b_j] in M that maximizes (b_j - n)
    4. Add the index of [a_j, b_j] to S and set n to b_j
    5. Remove [a_j, b_j] from future intervals
    6. If n != b (end of interval) then goto 2
    7. Output cardinality of S and S itself on two lines
*/
auto solve (double a, double b, vector<pair<double, double>> intervals) {
    vector<bool> used_intervals(intervals.size(), false); // Keep track of what intervals are already in the solution

    double n = a;
    vector<int> solution; // S Here
    while (true) {
        // if a == b choose the first interval that includes a
        if (a == b) {
            for (int i = 0; i < intervals.size(); ++i) {
                if (intervals[i].first <= a 
                    &&  intervals[i].second >= a) {
                        cout << 1 << "\n" << i << "\n";
                        return;
                }
            }
        }

        int max_index = NOT_FOUND;
        double max_distance = -1;
        double new_n = 0;
        for (int i = 0; i < intervals.size(); ++i) { // We don't need to actually construct M to find the max
            if (used_intervals[i]) { continue; }

            double a_j = intervals[i].first;
            double b_j = intervals[i].second;
            if (a_j <= n) {
                double distance = b_j - n;

                if (distance > max_distance) {
                    max_distance = distance;
                    max_index = i;
                    new_n = b_j;
                }
            }
        }
        if (max_index == NOT_FOUND) { // Check if M is empty
            cout << "impossible\n";
            return;
        }
        used_intervals[max_index] = true; // Mark interval as used

        solution.push_back(max_index); // Update solution and n
        n = new_n;
        if (n >= b) {
            break;
        }
    }
    // Print result
    cout << solution.size() << "\n";
    for (int i = 0; i < solution.size(); ++i) {
        cout << solution[i];
        if (i < solution.size() - 1) {
            cout << " ";
        }
    }
    cout << "\n";
}

// Main just reads the main numeric intervals + cover intervals
auto main () -> int {
    double a, b; 
    while (cin >> a >> b) {
        int nr_of_intervals;
        cin >> nr_of_intervals;
        vector<pair<double, double>> intervals(nr_of_intervals);
        cin.clear();
        for (int i = 0; i < nr_of_intervals; ++i) {
            double a_i, b_i;
            cin >> a_i; cin.clear();
            cin >> b_i;
            intervals[i] = make_pair(a_i, b_i);
        }
        solve(a, b, intervals);
    }
    return 0;
}