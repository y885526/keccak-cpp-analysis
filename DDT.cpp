#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;
using Matrix = vector<vector<int>>;

/**
 * Keccak Chi transformation (5-bit S-box).
 * This is the primary non-linear component 
 * of the Keccak permutation.
 */
uint32_t chi(uint32_t in) {
    uint32_t out = 0;
    uint32_t x[5];
    
    // Extract individual bits
    for(int i=0; i<5; i++) x[i] = (in >> i) & 1;
    
    // Apply the algebraic formula: 
    // y_i = x_i XOR ((NOT x_{i+1}) AND x_{i+2})
    for(int i=0; i<5; i++) {
        uint32_t bit = x[i] ^ ((x[(i+1)%5] ^ 1) & x[(i+2)%5]);
        out |= (bit << i);
    }
    return out;
}

/**
 * Calculates the standard Differential Distribution Table (DDT).
 * The matrix M[i][j] stores the number of times an input 
 * difference 'i' leads to an output difference 'j'.
 */
Matrix calculateDDT() {
    Matrix M(32, vector<int>(32, 0));

    // 'i' is the input difference (din), 'j' is the output difference (dout)
    for(uint32_t i = 0; i < 32; i++){
        for(uint32_t j = 0; j < 32; j++){
            int n = 0;
            
            // Iterate over all possible 5-bit inputs x
            for(uint32_t x = 0; x < 32; x++) {
                uint32_t x_prime = x ^ i;
                
                // Check if the actual output difference matches 'j'
                if ((chi(x) ^ chi(x_prime)) == j) {
                    n++;
                }
            }
            
            // Store the frequency directly at the corresponding indices
            M[i][j] = n;
        }
    }
    return M;
}

int main() {
    // Calculate the DDT once and store the result
    Matrix ddtResult = calculateDDT();

    // Output the matrix in a standard comma-separated format
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            cout << ddtResult[i][j] << (j == 31 ? "" : ",");
        }
        cout << endl;
    }

    return 0;
}
