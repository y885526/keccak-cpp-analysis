#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

// C++98 相容宣告
typedef vector<vector<int> > MatrixType2D;

// 您的 32x32 陣列 (請將您完整的資料貼在這裡)
MatrixType2D matrix = {
    {32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,8,8,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,4,4,0,0,4,4,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,8,8,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0},
    {0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0},
    {0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,4,4,0},
    {0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0},
    {0,0,0,0,0,4,0,4,0,0,0,0,0,4,0,4,4,0,4,0,0,0,0,0,4,0,4,0,0,0,0,0},
    {0,2,0,2,2,0,2,0,0,2,0,2,2,0,2,0,0,2,0,2,2,0,2,0,0,2,0,2,2,0,2,0},
    {0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0},
    {0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0},
    {0,4,0,4,4,0,4,0,0,0,0,0,0,0,0,0,0,4,0,4,4,0,4,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,4,0,4,0,0,4,0,4,4,0,4,0,0,4,0,4,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4},
    {0,0,2,2,0,0,2,2,2,2,0,0,2,2,0,0,0,0,2,2,0,0,2,2,2,2,0,0,2,2,0,0},
    {0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
    {0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0},
    {0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0},
    {0,0,0,0,4,0,0,4,4,0,0,4,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0},
    {0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,0,0,4,0,0,0,0,4,0,0,4},
    {0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2},
    {0,0,0,4,0,0,0,4,4,0,0,0,4,0,0,0,0,0,4,0,0,0,4,0,0,4,0,0,0,4,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,4,4,0,0,4,4},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8}
};

// 將整數轉換為 10-bit 二進位字串
string toBinaryString(int val) {
    string str = "";
    for (int i = 9; i >= 0; i--) {
        str += ((val >> i) & 1) ? "1" : "0";
    }
    return str;
}

// 判斷兩個字串是否只差 1 個 bit，並回傳融合後的結果 (不同的 bit 變成 '-')
bool tryMerge(const string& a, const string& b, string& merged) {
    int diff_count = 0;
    merged = a;
    for (int i = 0; i < 10; i++) {
        if (a[i] != b[i]) {
            diff_count++;
            merged[i] = '-';
        }
    }
    return (diff_count == 1);
}

// 檢查某個質隱項是否涵蓋某個原始 0
bool covers(const string& pi, const string& minterm) {
    for (int i = 0; i < 10; i++) {
        if (pi[i] != '-' && pi[i] != minterm[i]) return false;
    }
    return true;
}

// 將帶有 '-' 的 10-bit 字串轉換為 CNF 子句
string toCNFClause(const string& pi) {
    string vars[] = {"y4", "y3", "y2", "y1", "y0", "x4", "x3", "x2", "x1", "x0"};
    string clause = "(";
    bool first = true;
    for (int i = 0; i < 10; i++) {
        if (pi[i] == '0') {
            if (!first) clause += " + ";
            clause += vars[i];  // 0 寫原變數
            first = false;
        } else if (pi[i] == '1') {
            if (!first) clause += " + ";
            clause += vars[i] + "'"; // 1 寫反相變數
            first = false;
        }
    }
    clause += ")";
    return clause;
}

// --- 全域變數供 DFS 搜尋使用 ---
int best_solution_size = 9999;
vector<int> best_solution_indices;
vector<string> all_PIs;
vector<string> original_minterms;
vector<vector<int> > minterm_to_PIs; // 記錄每個 0 被哪些 PI 涵蓋

// 深度優先搜尋尋找最小覆蓋 (Algorithm X 精神)
void solveExactCover(int minterm_idx, vector<int>& current_selection, vector<int>& cover_counts, int covered_total) {
    // 剪枝：如果目前的組合已經比已知的最佳解差，直接放棄
    if (current_selection.size() >= best_solution_size) return;

    // 成功條件：所有原始的 0 都被覆蓋了
    if (covered_total == original_minterms.size()) {
        best_solution_size = current_selection.size();
        best_solution_indices = current_selection;
        cout << ">> [發現更佳解] 目前最少需要: " << best_solution_size << " 個 CNF 子句" << endl;
        return;
    }

    // 尋找第一個「還沒被覆蓋」的 0
    while (minterm_idx < original_minterms.size() && cover_counts[minterm_idx] > 0) {
        minterm_idx++;
    }
    
    // 如果找完發現全部都覆蓋了 (防呆)
    if (minterm_idx >= original_minterms.size()) return;

    // 取得所有可以覆蓋這個 0 的 質隱項 (PI)
    const vector<int>& covering_PIs = minterm_to_PIs[minterm_idx];
    
    // 嘗試每一個能覆蓋它的 PI
    for (size_t i = 0; i < covering_PIs.size(); i++) {
        int pi_index = covering_PIs[i];
        
        // 記錄這個 PI 新覆蓋了幾個 0
        int new_covered = 0;
        vector<int> newly_covered_indices;
        
        for (size_t m = 0; m < original_minterms.size(); m++) {
            if (covers(all_PIs[pi_index], original_minterms[m])) {
                if (cover_counts[m] == 0) {
                    new_covered++;
                    newly_covered_indices.push_back(m);
                }
                cover_counts[m]++;
            }
        }

        current_selection.push_back(pi_index);
        
        // 遞迴進入下一層
        solveExactCover(minterm_idx + 1, current_selection, cover_counts, covered_total + new_covered);
        
        // 回溯 (Backtrack) 恢復狀態
        current_selection.pop_back();
        for (size_t m = 0; m < original_minterms.size(); m++) {
            if (covers(all_PIs[pi_index], original_minterms[m])) {
                cover_counts[m]--;
            }
        }
    }
}

int main() {
    // 1. 收集所有為 0 的狀態 (轉換為 10-bit 字串)
    set<string> current_groups;
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            if (matrix[y][x] == 0) {
                // 假設您的矩陣行列就是純二進位對應。
                // 如果是格雷碼，需要在此處加入轉換。
                int val = (y << 5) | x; 
                string binStr = toBinaryString(val);
                current_groups.insert(binStr);
                original_minterms.push_back(binStr);
            }
        }
    }

    cout << "總共有 " << original_minterms.size() << " 個 0 需要化簡..." << endl;
    cout << "階段 1: Q-M 演算法尋找質隱項 (Prime Implicants)..." << endl;

    // 2. Q-M 演算法核心：不斷融合
    set<string> prime_implicants;
    bool merged_any = true;
    
    while (merged_any) {
        merged_any = false;
        set<string> next_groups;
        set<string> used_in_this_round;

        vector<string> group_list(current_groups.begin(), current_groups.end());
        
        for (size_t i = 0; i < group_list.size(); i++) {
            for (size_t j = i + 1; j < group_list.size(); j++) {
                string merged_str;
                if (tryMerge(group_list[i], group_list[j], merged_str)) {
                    next_groups.insert(merged_str);
                    used_in_this_round.insert(group_list[i]);
                    used_in_this_round.insert(group_list[j]);
                    merged_any = true;
                }
            }
        }

        // 把沒有被融合的保留下來，成為質隱項 (PI)
        for (size_t i = 0; i < group_list.size(); i++) {
            if (used_in_this_round.find(group_list[i]) == used_in_this_round.end()) {
                prime_implicants.insert(group_list[i]);
            }
        }
        current_groups = next_groups;
    }
    
    // 把最後一輪剩下的也都加進質隱項
    prime_implicants.insert(current_groups.begin(), current_groups.end());
    all_PIs.assign(prime_implicants.begin(), prime_implicants.end());

    cout << "找到 " << all_PIs.size() << " 個候選質隱項。" << endl;
    cout << "階段 2: 執行精確集合覆蓋 (Exact Cover 搜尋最小解)..." << endl;

    // 3. 建立覆蓋表
    minterm_to_PIs.resize(original_minterms.size());
    for (size_t m = 0; m < original_minterms.size(); m++) {
        for (size_t p = 0; p < all_PIs.size(); p++) {
            if (covers(all_PIs[p], original_minterms[m])) {
                minterm_to_PIs[m].push_back(p);
            }
        }
    }

    // 4. 開始搜尋最小組合
    vector<int> current_selection;
    vector<int> cover_counts(original_minterms.size(), 0);
    
    // 如果論文說最佳解是 46，您可以把初始 best_solution_size 設為 47 來加速剪枝
    best_solution_size = 47; 
    
    solveExactCover(0, current_selection, cover_counts, 0);

    // 5. 輸出最終 CNF 結果
    cout << "\n==================================================" << endl;
    if (best_solution_indices.empty()) {
        cout << "未能在限制數量內找到覆蓋所有 0 的組合。" << endl;
    } else {
        cout << "【化簡完成】完美找出 " << best_solution_indices.size() << " 個 CNF 子句！" << endl;
        cout << "CNF 數學式如下：" << endl;
        
        for (size_t i = 0; i < best_solution_indices.size(); i++) {
            string pi = all_PIs[best_solution_indices[i]];
            cout << "子句 " << i + 1 << ": " << toCNFClause(pi) << "  [代碼: " << pi << "]" << endl;
        }
    }

    return 0;
}
