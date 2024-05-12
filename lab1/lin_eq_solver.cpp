// Author: Viktor Bergström
// Problem: Linear Equation Solver

#include<iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>
#include <fstream>
#include <cmath>

using namespace std;

ofstream file("cpp_result.txt");

// Needed to avoid floating point errors
auto equals(double a, double b) -> bool {
    return abs(a - b) < 1e-9;
}

/*
    Perform gaussian elimination to solve a linear system of equations given by an augmented matrix.
    Aditiona
    Assumes aug is a n * (n+1) sized matrix
    Runs in O(n^3) because of the triple nested for loop when doing rref.
*/
auto gauss_elim(vector<vector<double>> aug) -> pair<int, vector<double>> {

    /*
        We wish to transform each row one by one such that it's first non-zero
        element is in the column index equal to it's row index. Additionally we want all
        element below and above this non-zero element(pivot) to be zero. 
        This will give us a matrix on reduced row echelon form from which we can easily read out 
        the solution.
    */
    for (int col = 0, row = 0; col < aug.size() && row < aug.size(); ++col) {

        /*
            Find the row with the greatest element in the position of the
            current rows pivot column.
        */
        int pivot_index = row;
        for (int i = row; i < aug.size(); ++i) {
            if (abs(aug[i][col]) > abs(aug[pivot_index][col])) {
                pivot_index = i;
            }
        }
        /* 
            If no pivot element greater than 0 was found then the pivot of this
            row has to be the next column
        */
        if (equals(aug[pivot_index][col], 0)) {
            continue;
        }
        /* 
            If we found a pivot swap that row and the current one
        */
        for (int i = col; i <= aug.size(); ++i) {
            swap (aug[pivot_index][i], aug[row][i]);
        }

        /* 
            Reduce all elements below and above the pivot to 0 by subtracting the row multiplied by a factor
            to each row bellow it.
        */
        for (int i = 0; i < aug.size(); ++i) {
            if (i != row) {
                double c = aug[i][col] / aug[row][col];
                for (int j = col; j <= aug.size(); ++j) {
                    aug[i][j] -= aug[row][j] * c;
                }
            }
        }
        ++row;
    }


    for (auto row : aug) {
        for (auto x : row) {
            cout << x << " ";
        }
        cout << endl;

    }

    /*
        Go row for row and read out the isolated pivot variable if possible
        If there is no pivot and rhs is non zero then we have no solutions
        If there are two variables in the lhs we have multiple.
    */
    vector<double> solution(aug.size(), numeric_limits<double>::infinity());
    bool inconsistent = false, multiple = false;
    for (int i=0; i < aug.size(); ++i) {
        int e = 0;
        int piv = 0;
        for (int j = 0; j < aug.size(); ++j) {
            if (!equals(aug[i][j], 0)) {
                if (e == 0) {
                    piv = j;
                }
                e++;
            }
        }
        if (e == 1) {
            solution[piv] = aug[i][aug.size()] / aug[i][piv];
        }
        if (e == 0) {
            if (!equals(aug[i][aug.size()], 0)) {
                inconsistent = true;
            }
        }
        if (e > 1) {
            multiple = true;
        }
    }
    
    if (inconsistent) {
        return make_pair(0, solution);
    }
    else if (multiple) {
        return make_pair(-1, solution);
    }
    else {
        return make_pair(1, solution);
    }
}

auto main() -> int {
    int n;
    while (true) {
        cin >> n;
        if (!n) { break; }

        vector<vector<double>> aug(n, vector<double>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> aug[i][j];
            }
        }
        for (int i = 0; i < n; ++i) {
            double vec_i;
            cin >> vec_i;
            aug[i].push_back(vec_i);
        }
        stringstream output;
        auto [nr_of_solutions, solution] = gauss_elim(aug);
        if (nr_of_solutions == 0) {
            file << "inconsistent\n";
        }
        else  if (nr_of_solutions == -1) {
            for (auto x : solution) {
                if (x != numeric_limits<double>::infinity())  {
                    output << setprecision(3) << fixed << x << " ";
                }
                else {
                    output << "? ";
                }
            }
            file << output.str() << endl;
        }
        else if (nr_of_solutions == 1) {
            for (auto x : solution) {
                output << setprecision(3) << fixed << x << " ";
            }
            file << output.str() << endl;
        }
    }
    return 0;
}