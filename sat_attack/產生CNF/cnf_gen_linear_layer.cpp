#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <cmath>

// 這個類別專門用來收集 CNF 子句，並寫出成標準的 .cnf 檔案
class CNFWriter {
public:
    std::vector<std::vector<int>> clauses;
    int max_var = 0;

    void add_clause(const std::vector<int>& clause) {
        clauses.push_back(clause);
        for (int lit : clause) {
            int v = std::abs(lit);
            if (v > max_var) max_var = v;
        }
    }

    void write_to_file(const std::string& filename) {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "無法開啟檔案寫入！" << std::endl;
            return;
        }
        // 寫入 DIMACS 標準檔頭: p cnf <變數總數> <子句總數>
        out << "p cnf " << max_var << " " << clauses.size() << "\n";
        
        // 寫入所有子句，每個子句以 0 結尾
        for (const auto& clause : clauses) {
            for (int lit : clause) {
                out << lit << " ";
            }
            out << "0\n";
        }
        out.close();
        std::cout << "成功寫入檔案: " << filename << std::endl;
        std::cout << "總變數數量: " << max_var << ", 總子句數量: " << clauses.size() << std::endl;
    }
};

inline int mod(int a, int b) {
    return (a % b + b) % b;
}

int popcount(uint32_t n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

void addXorAsCNF(CNFWriter& writer, const std::vector<uint32_t>& vars) {
    int n = vars.size();
    int num_combinations = 1 << n; 
    
    for (int mask = 0; mask < num_combinations; ++mask) {
        if (popcount(mask) % 2 != 0) {
            std::vector<int> clause;
            for (int i = 0; i < n; ++i) {
                int var = vars[i];
                if ((mask >> i) & 1) {
                    clause.push_back(-var); 
                } else {
                    clause.push_back(var);
                }
            }
            writer.add_clause(clause);
        }
    }
}

const int T[5][5] = {
    {0, 36, 3, 41, 18}, {1, 44, 10, 45, 2}, {62, 6, 43, 15, 61}, 
    {28, 55, 25, 21, 56}, {27, 20, 39, 8, 14}
};

const uint32_t OFFSET_ALPHA   = 1;
const uint32_t OFFSET_BETA    = 1601;
const uint32_t OFFSET_COLSUM  = 3201; 

uint32_t getAlphaVar(int i, int j, int k) { return OFFSET_ALPHA + (320 * j + 64 * i + k); }
uint32_t getBetaVar(int i, int j, int k)  { return OFFSET_BETA + (320 * j + 64 * i + k); }
uint32_t getColSumVar(int i, int k)       { return OFFSET_COLSUM + (64 * i + k); }

// 目標輸出狀態 (Beta)
const uint64_t target_output[25] = {
0x0000000000000001,0x0000000000000000,
0x0000000000000008,0x0000000000000000,
0x0000000000000000,0x0000000000000008,
0x0000000000000000,0x0000000000000008,
0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000040000000,
0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000040000000,
0x0000000000000000,0x0000000000000000,
0x0000000000000000
};

int main() {
    CNFWriter writer;
    std::vector<uint32_t> xor_vars;

    std::cout << "開始生成 Keccak-f 的線性層 CNF..." << std::endl;

    // 步驟 1: ColumnSum
    for (int i = 0; i < 5; ++i) {
        for (int k = 0; k < 64; ++k) {
            xor_vars.clear();
            xor_vars.push_back(getColSumVar(i, k)); 
            for (int j = 0; j < 5; ++j) {
                xor_vars.push_back(getAlphaVar(i, j, k)); 
            }
            addXorAsCNF(writer, xor_vars);
        }
    }

    // 步驟 2: beta 映射
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 64; ++k) {
                xor_vars.clear();

                int mapped_i = j;
                int mapped_j = mod(2 * i + 3 * j, 5);
                int mapped_k = mod(k - T[i][j], 64);

                xor_vars.push_back(getBetaVar(mapped_i, mapped_j, mapped_k));
                xor_vars.push_back(getAlphaVar(i, j, k));
                xor_vars.push_back(getColSumVar(mod(i - 1, 5), k));
                xor_vars.push_back(getColSumVar(mod(i + 1, 5), mod(k - 1, 64)));

                addXorAsCNF(writer, xor_vars);
            }
        }
    }

    // 步驟 3: 加入目標輸出 (Beta) 的單元子句限制
    std::cout << "加入目標輸出狀態 (Beta) 的 Unit Clauses..." << std::endl;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            // 注意對應順序：這裡假設陣列是按 row-major (j * 5 + i) 或 column-major 存放。
            // 由於 Keccak state A[x,y] 通常在 1D 陣列表示為 A[5*y + x]，
            // 這裡使用 j * 5 + i 作為索引。如果你的陣列定義不同，請改為 i * 5 + j。
            int lane_index = j * 5 + i; 
            uint64_t lane_val = target_output[lane_index];
            
            for (int k = 0; k < 64; ++k) {
                uint32_t beta_var = getBetaVar(i, j, k);
                int bit_val = (lane_val >> k) & 1;
                
                // 如果目標位元是 1，加入子句 [var]；如果是 0，加入 [-var]
                if (bit_val == 1) {
                    writer.add_clause({(int)beta_var});  
                } else {
                    writer.add_clause({-(int)beta_var}); 
                }
            }
        }
    }

    // 將結果輸出成標準的文字檔
    writer.write_to_file("keccak_linear_with_output.cnf");

    return 0;
}
