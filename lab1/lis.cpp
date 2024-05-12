// Author: Viktor Bergström (vikbe588)
// Problem: lis (longest increasing subsequence)

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

// Struct used for aux vector 
struct Previous {
    Previous* p;
    int index;
};

/*
    Algorithm: Use patient sort to find the lis of a sequence with a time complexity of O(nlog(n))
*/
auto longest_increasing_subsequence(vector<int> &sequence) -> vector<int> {

    // Auxillary vector for restoring the solution at the end
    vector<Previous*> previous(sequence.size());
    
    vector<int> piles(sequence.size()+1, numeric_limits<int>::max()); // Each index represents the top element in a pile
    piles[0] = -numeric_limits<int>::max();
    for (int i = 0; i < sequence.size(); ++i) {

        /* 
            Find the index to potentially place the element, which should be the leftmost pile
            which value is greater than that of the element, which is equivalent to doing an 
            upper bound search in the piles for sequence[i];

            Subtracting piles.begin() to get the index of the upper bound for sequence[i] piles 
        */
        int pile_index = upper_bound(piles.begin(), piles.end(), sequence[i]) - piles.begin(); 

        /* 
            If element is less than the top of the current pile and greater than the previous pile 
            We should add it to the current pile
        */
        if (piles[pile_index-1] < sequence[i] && sequence[i] < piles[pile_index]) {
            piles[pile_index] = sequence[i];
            // Save data for restoring later
            previous[pile_index] = new Previous();
            previous[pile_index]->p = previous[pile_index-1];
            previous[pile_index]->index = i;
        }
    }

    // Get the length of the lis
    int lis = 0;
    for (int i = 0; i <= sequence.size(); i++) {
        if (piles[i] < numeric_limits<int>::max())
            lis = i;
    }

    // Follow the chain backwards from the last element until we find a null pointer
    vector<int> indices;
    Previous *p = previous[lis];
    while (true) {
        indices.push_back(p->index);
        p = p->p;
        if (p == nullptr) {
            break;
        }
    }
    reverse(indices.begin(), indices.end());
    return indices;
}

auto main () -> int {

    int sequence_len;
    while (cin >> sequence_len) {
        vector<int> sequence(sequence_len);
        for (int i = 0; i < sequence_len; ++i) {
            cin >> sequence[i];
        }
        auto indicies = longest_increasing_subsequence(sequence);
        cout << indicies.size() << '\n';
        stringstream ss;
        for (auto i : indicies) {
            ss << i << " ";
        }
        cout << ss.str() << endl;
    }
    return 0;
}
