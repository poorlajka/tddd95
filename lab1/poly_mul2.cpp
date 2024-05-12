// Author: Viktor Bergström (vikbe588)
// Problem: Polynomial Multiplication 2 

#include <iostream>
#include <vector>
#include <complex>
#include <sstream>
#include <cmath>
#include <fstream>

using namespace std;

/*
    fast fourier transforms which computes the discrete fourier transform of a signal
    Runs in O(nlog(n))

    The time complexity is gained from splitting the dft into two 
    smaller dfts recursivly. The summation of the bottom(which runs in O(n)) uses the results of these
    recursive calls.
*/
using cd = complex<double>;
const double PI = acos(-1);

void fft(vector<cd> & signal, bool invert) {
    int n = signal.size();
    if (n == 1)
        return;

    vector<cd> signal_even(n / 2), signal_odd(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        signal_even[i] = signal[2*i];
        signal_odd[i] = signal[2*i+1];
    }

    fft(signal_even, invert);
    fft(signal_odd, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    cd w(1), wn(cos(ang), sin(ang));
    for (int i = 0; 2 * i < n; i++) {
        signal[i] = signal_even[i] + w * signal_odd[i];
        signal[i + n/2] = signal_even[i] - w * signal_odd[i];
        if (invert) {
            signal[i] /= 2;
            signal[i + n/2] /= 2;
        }
        w *= wn;
    }
}

/* 
    Algorithm:

    According to the convolution theorem we know that:
        Convolution in the time domain <=> pairwise multiplication in the frequency domain     

    As such if we have to polynomials in the form of coefficient vectors and want to 
    multiply them (convolution between the two vectors/signals). We can instead compute the pairwise multiplication of 
    their frequency domain representations.

    To do this we need to first perform a discrete fourier transform on each of the vectors
    for which we will use the FFT algorithm.
*/
auto multiply(vector<cd> x1, vector<cd> x2) -> vector<cd> {
    int new_degree = (x1.size()-1) + (x2.size()-1);

    /*
        To do pairwise multiplication later on the frequency domain representation of
        x1 and x2 need to be of the same length. Additionally their size must be greater or equal their sum in order
        to hold the degree of the new polynomial after multiplication. Lastly for the FFT algorithm to work the size
        needs to be a power of 2.
    */
    int new_size = 1;
    while (new_size < x1.size() + x2.size()) 
        new_size *=2;
    x1.resize(new_size);
    x2.resize(new_size);

    fft(x1, false);
    fft(x2, false);

    vector<cd> pairwise_mult;
    for (int i = 0; i < x1.size(); ++i) {
        pairwise_mult.push_back(x1[i]*x2[i]);
    }
    fft(pairwise_mult, true);

    //Adjust to only return the part of the vector we want a.k.a [x^0 .. x^degree]
    return vector<cd>(pairwise_mult.begin(), pairwise_mult.begin() + new_degree+1);
}
        
auto main() -> int {

    int _; //There is only one test case but we still need to read the 1
    cin >> _;

    int x1_deg;
    cin >> x1_deg;
    vector<cd> x1(x1_deg+1);
    for (int i = 0; i < x1_deg+1; ++i) {
        cin.clear();
        cin >> x1[i];
    }

    int x2_deg;
    cin >> x2_deg;
    vector<cd> x2(x2_deg+1);
    for (int i = 0; i < x2_deg+1; ++i) {
        cin.clear();
        cin >> x2[i];
    }

    vector<cd> product = multiply(x1, x2);

    stringstream answer;
    answer << product.size()-1 << "\n";
    for (auto term : product) {
        answer << static_cast<int>(round(real(term))) << " ";
    }
    answer << "\n";
    //ofstream file("cpp_result.txt");
    cout << answer.str();
    return 0;
}

