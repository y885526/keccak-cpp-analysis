#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

// === 全域變數 ===
int var_counter = 0;
vector<vector<int>> clauses; 

// === 輔助函數 ===

int newVar() {
    return ++var_counter;
}

void add_clause(const vector<int>& lits) {
    clauses.push_back(lits);
}

// Keccak S-box (Chi) 邏輯
uint32_t chi(uint32_t in) {
    uint32_t out = 0;
    uint32_t x[5];
    for(int i=0; i<5; i++) x[i] = (in >> i) & 1;
    for(int i=0; i<5; i++) {
        uint32_t bit = x[i] ^ ((x[(i+1)%5] ^ 1) & x[(i+2)%5]);
        out |= (bit << i);
    }
    return out;
}

// 驗證差分是否合法
bool is_valid_differential(uint32_t din, uint32_t dout) {
    for(uint32_t x = 0; x < 32; x++) {
        uint32_t x_prime = x ^ din;
        uint32_t y = chi(x);
        uint32_t y_prime = chi(x_prime);
        if ((y ^ y_prime) == dout) return true;
    }
    return false;
}

// 生成 S-box 約束
void add_sbox_constraints(const vector<int>& din_vars, const vector<int>& dout_vars) {
    for(uint32_t d_in_val = 0; d_in_val < 32; d_in_val++) {
        for(uint32_t d_out_val = 0; d_out_val < 32; d_out_val++) {
            if (!is_valid_differential(d_in_val, d_out_val)) {
                vector<int> clause;
                for(int i=0; i<5; i++) {
                    bool bit_is_1 = (d_in_val >> i) & 1;
                    if (bit_is_1) clause.push_back(-din_vars[i]);
                    else          clause.push_back(din_vars[i]);
                }
                for(int i=0; i<5; i++) {
                    bool bit_is_1 = (d_out_val >> i) & 1;
                    if (bit_is_1) clause.push_back(-dout_vars[i]);
                    else          clause.push_back(dout_vars[i]);
                }
                add_clause(clause);
            }
        }
    }
}

int main() {
    // === 1. 建立變數 (1600 bit 狀態) ===
    vector<int> din_vars(1600);
    vector<int> dout_vars(1600);
    
    // 建立 1600 個輸入差分變數 (ID: 1 ~ 1600)
    for(int i=0; i<1600; i++) {
        din_vars[i] = newVar();  
    }
    
    // 建立 1600 個輸出差分變數 (ID: 1601 ~ 3200)
    for(int i=0; i<1600; i++) {
        dout_vars[i] = newVar(); 
    }
    
    cout << "變數建立完成。輸入 IDs: " << din_vars[0] << "~" << din_vars[1599] 
         << ", 輸出 IDs: " << dout_vars[0] << "~" << dout_vars[1599] << endl;

    // === 2. 生成 320 個 S-box 的行為約束 ===
    cout << "正在生成 320 個 S-box 的 CNF 約束..." << endl;
    for(int s = 0; s < 320; s++) {
        vector<int> current_din(5);
        vector<int> current_dout(5);
        
int y = s / 64;
int z = s % 64;

for(int x = 0; x < 5; x++) {
    int actual_index = 320 * y + 64 * x + z;
    current_din[x] = din_vars[actual_index];
    current_dout[x] = dout_vars[actual_index];
}
        
        add_sbox_constraints(current_din, current_dout);
    }

    // === 3. 排除全 0 解 (Non-Zero Constraint) ===
    vector<int> non_zero_clause;
    for(int i=0; i<1600; i++) {
        non_zero_clause.push_back(din_vars[i]);
    }
    add_clause(non_zero_clause);
    cout << "已加入限制：1600-bit 輸入差分不能全為 0" << endl;

    // === 4. 鎖定特定輸出 (Fix Output) ===
    string target_output = "1011001010110110000010110101111100010011101000110011011000001011010110100010111000100110100011011100110111011000000110001110010001111000101010101010011101110010111010001111110000110110000000100110100001100110111011010110000111001011101010001110001101010110011001001101100110100110011100111100010100100110011000010001110101110110110000001011011101110111010001001011111011111001011000000011000000000111110011011000010110010100100110100110111101010110001001000011000101000101111110001000101000100101111110010111000101011011111101011100110011001001010011111011110000111011001000000010100000111111110101101001111000010111001001100000110000010011101100010010111100110100001111011001011100101111101101100010001010101001100010101011111110001000000100101011000110001110001100001010000011111101111011100111100001100010010111010011101101011001111101101001101110111111001011101000110110000110011100110011010101001001010101001101110101011111100001100111110000110001111110110110011100001110100101000011111010000110100010110110011000100110011100010110011001111100000100001101011010010000000001011101001001101110001011111011111110110110110110000111110001110111000110000100011001000111100110000100110000111010000001010011000111110010010100010100011000101000111100100000101101001101001110011101100011010111101000001011110010100001111100110011011010111011010010100001101111111010000010010010110100011011111000110101000011000111011010111110101100101010011010001001100011000110010000111111110100010001111010100101100001000100011010100100111101100101111111000110001101001100100010111110001110001010011101100000011001011100";
    
    if (target_output.length() == 1600) {
        cout << "正在鎖定 1600-bit 輸出差分..." << endl;
        for(int i = 0; i < 1600; i++) {
            int var_id = dout_vars[i];
            vector<int> unit_clause;
            
            if (target_output[i] == '1') {
                unit_clause.push_back(var_id);
            } else {
                unit_clause.push_back(-var_id);
            }
            
            add_clause(unit_clause);
        }
    } else {
        cout << "錯誤：提供的輸出字串長度為 " << target_output.length() << "，應為 1600！" << endl;
    }

    // === 5. 輸出為 .cnf 檔案 (DIMACS 格式) ===
    ofstream outfile("attack.cnf");
    
    outfile << "p cnf " << var_counter << " " << clauses.size() << endl;
    
    for(const auto& cls : clauses) {
        for(int lit : cls) {
            outfile << lit << " ";
        }
        outfile << "0\n"; 
    }
    
    outfile.close();
    
    cout << "CNF 檔案 'attack.cnf' 生成成功！" << endl;
    cout << "變數總數: " << var_counter << endl;
    cout << "子句總數: " << clauses.size() << endl;
    
    return 0;
}
