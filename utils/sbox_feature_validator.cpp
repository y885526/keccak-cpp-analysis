#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;



// 3. 定義 Keccak S-box (Chi) 邏輯
// 回傳 y = Chi(x)
uint32_t chi(uint32_t in) {
    uint32_t out = 0;
    uint32_t x[5];
    for(int i=0; i<5; i++) x[i] = (in >> i) & 1;
    for(int i=0; i<5; i++) {
        // Chi: y_i = x_i ^ ((~x_{i+1}) & x_{i+2})
        uint32_t bit = x[i] ^ ((x[(i+1)%5] ^ 1) & x[(i+2)%5]);
        out |= (bit << i);
    }
    return out;
}

// 4. 驗證差分是否合法
void V(uint32_t din, uint32_t dout) {
	uint32_t x_1[5];
	uint32_t x_2[5];
	uint32_t x_3[5];
    // 窮舉所有可能的原狀態 x (0~31)
    for(uint32_t x = 0; x < 32; x++) {
        uint32_t x_prime = x ^ din;
        uint32_t y = chi(x);
        uint32_t y_prime = chi(x_prime);
        // 如果找到一組 x，使得輸出差分等於我們想要的 dout
        if ((y ^ y_prime) == dout){
        	for(int i=0; i<5; i++) x_1[i] = (x >> i) & 1;
        	for(int i=0; i<5; i++) x_2[i] = (din>> i) & 1;
        	for(int i=0; i<5; i++) x_3[i] = (dout>> i) & 1;
        	for(int i = 0; i < 5; i++) cout<< x_1[i]  ;
        	cout<<" " ;
        	for(int i = 0; i < 5; i++) cout<< x_2[i]  ;
        	for(int i = 0; i < 5; i++) cout<< x_3[i]  ; 
        	cout<<std::endl;
        	
		}
    }
}



int main(){
	for(uint32_t y = 0; y < 32; y++)
	for(uint32_t x = 0; x < 32; x++)
	V(x,y);
	
	return 0;
	
}
		
		
	

