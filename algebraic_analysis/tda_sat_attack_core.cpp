#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;
using MatrixType2D = vector<vector<int>>;

// === [定義區] 固定的陣列與變數 ===
int var_counter = 0;
vector<vector<int>> base_sbox_clauses; // 暫存單一 S-box 的所有子句
vector<vector<int>> linear_clauses;    // 暫存線性層的所有子句
vector<vector<int>> blocking_clauses;  // 存放每一次失敗的 beta_0 (Blocking Clauses)

// alpha_1 (DDT=4_2_Test 中使用)
const vector<int> alpha_1 = {
0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,0,0,0,1,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,0,1,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1
};

// alpha_2 (固定輸出限制, 第一個檔案中的定義)
const int alpha_2[1600] = {
0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,0,0,0,1,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,0,1,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1
};

MatrixType2D Alpha_1(320, vector<int>(5, 0));
MatrixType2D Beta_0(320, vector<int>(5, 0));

MatrixType2D matrix_lookup = {
{32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,8,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
{0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0},
{0,0,0,0,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,4,0,4,0,0,0,0,0,4,0,4,0,0,0,0,0,0,4,0,4,0,0,0,0,0,4,0,4},
{0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
{0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2},
{0,0,0,0,0,0,0,0,8,0,8,0,8,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,4,0,4,0,0,0,0,0,0,0,0,0,4,0,4,0,4,0,4,0,0,0,0,0,0,0,0,0,4,0,4},
{0,0,0,0,0,0,0,0,4,0,0,4,4,0,0,4,0,0,0,0,0,0,0,0,4,0,0,4,4,0,0,4},
{0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0},
{0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,4,0,4,0,4,0,4,0,0,0,0,0,0,0,0,0,0,4,0,4,0,4,0,4,0,0,0,0},
{0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2},
{0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,8,0,0,0,8,0,0,0,8,0,0,0},
{0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0},
{0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4},
{0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,4,4,4,4,0,0,0,0,4,4},
{0,4,0,0,0,0,0,4,0,4,0,0,0,0,0,4,4,0,0,0,0,0,4,0,4,0,0,0,0,0,4,0},
{0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0},
{0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0},
{0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2},
{0,0,0,0,0,0,0,0,4,0,0,4,4,0,0,4,4,0,0,4,4,0,0,4,0,0,0,0,0,0,0,0},
{0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
{0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0},
{0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0},
{0,2,2,0,2,0,0,2,2,0,0,2,0,2,2,0,2,0,0,2,0,2,2,0,0,2,2,0,2,0,0,2}
};

// ==========================================
// === [新增] 線性層 (Theta, Pi, Rho) 相關 ===
// ==========================================

const int T[5][5] = {
    {0, 36, 3, 41, 18}, {1, 44, 10, 45, 2}, {62, 6, 43, 15, 61}, 
    {28, 55, 25, 21, 56}, {27, 20, 39, 8, 14}
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

void addXorAsCNF(vector<vector<int>>& clauses_list, const vector<uint32_t>& vars) {
    int n = vars.size();
    int num_combinations = 1 << n; 
    for (int mask = 0; mask < num_combinations; ++mask) {
        if (popcount(mask) % 2 != 0) {
            vector<int> clause;
            for (int i = 0; i < n; ++i) {
                int var = vars[i];
                if ((mask >> i) & 1) clause.push_back(-var); 
                else clause.push_back(var);
            }
            clauses_list.push_back(clause);
        }
    }
}

// 變數 ID 映射：
// Alpha   (輸入): 1 ~ 1600
// ColSum  (輔助): 1601 ~ 1920
// Beta    (輸出): 1921 ~ 3520 (這個段落直接接上 S-box 的輸入)
uint32_t getAlphaVar(int i, int j, int k) { return 1 + (320 * j + 64 * i + k); }
uint32_t getColSumVar(int i, int k)       { return 1601 + (64 * i + k); }
uint32_t getBetaVar(int i, int j, int k)  { return 1921 + (320 * j + 64 * i + k); }

void precompute_linear_layer_clauses() {
    // 1. ColumnSum
    for (int i = 0; i < 5; ++i) {
        for (int k = 0; k < 64; ++k) {
            vector<uint32_t> xor_vars;
            xor_vars.push_back(getColSumVar(i, k)); 
            for (int j = 0; j < 5; ++j) {
                xor_vars.push_back(getAlphaVar(i, j, k)); 
            }
            addXorAsCNF(linear_clauses, xor_vars);
        }
    }

    // 2. Beta 映射 (Theta, Pi, Rho)
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 64; ++k) {
                vector<uint32_t> xor_vars;
                int mapped_i = j;
                int mapped_j = mod(2 * i + 3 * j, 5);
                int mapped_k = mod(k + T[i][j], 64);

                xor_vars.push_back(getBetaVar(mapped_i, mapped_j, mapped_k));
                xor_vars.push_back(getAlphaVar(i, j, k));
                xor_vars.push_back(getColSumVar(mod(i - 1, 5), k));
                xor_vars.push_back(getColSumVar(mod(i + 1, 5), mod(k - 1, 64)));

                addXorAsCNF(linear_clauses, xor_vars);
            }
        }
    }
}


// === [輔助函數] S-box CNF 相關 ===
int newVar() { return ++var_counter; }
void add_clause(const vector<int>& lits) { base_sbox_clauses.push_back(lits); }

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

bool is_valid_differential(uint32_t din, uint32_t dout) {
    for(uint32_t x = 0; x < 32; x++) {
        uint32_t x_prime = x ^ din;
        uint32_t y = chi(x);
        uint32_t y_prime = chi(x_prime);
        if ((y ^ y_prime) == dout) return true;
    }
    return false;
}

void precompute_base_sbox_constraints() {
    vector<int> din_vars, dout_vars;
    // 預先產生原型變數 (ID 1~10)
    for(int i=0; i<5; i++) din_vars.push_back(newVar());  
    for(int i=0; i<5; i++) dout_vars.push_back(newVar()); 

    for(uint32_t d_in_val = 0; d_in_val < 32; d_in_val++) {
        for(uint32_t d_out_val = 0; d_out_val < 32; d_out_val++) {
            if (!is_valid_differential(d_in_val, d_out_val)) {
                vector<int> clause;
                for(int i=0; i<5; i++) {
                    if ((d_in_val >> i) & 1) clause.push_back(-din_vars[i]);
                    else                     clause.push_back(din_vars[i]);
                }
                for(int i=0; i<5; i++) {
                    if ((d_out_val >> i) & 1) clause.push_back(-dout_vars[i]);
                    else                      clause.push_back(dout_vars[i]);
                }
                add_clause(clause);
            }
        }
    }
}

// 【修改】將原本的 S-box 變數整體往後平移 1920
// 原本輸入 1~1600   -> 變成 1921~3520 (接續線性層的 Beta 輸出)
// 原本輸出 1601~3200 -> 變成 3521~5120
int f_idx(int x){
    if(x <= 5) return 1920 + x*64 - 63;
    else return 3520 + (x - 5)*64 - 63;
}

void generate_attack1600_cnf(const vector<vector<int>>& blocks) {
    // 總變數 = Alpha(1600) + ColSum(320) + Beta/Sbox-in(1600) + Sbox-out(1600) = 5120
    int total_vars = 5120;
    
    // 子句總數 = S-box規則(單個*320) + Alpha2目標綁定(1600) + Alpha輸入補零(262) + 線性層子句 + 阻擋子句
    int total_clauses = (base_sbox_clauses.size() * 320) + 1600 + 262 + linear_clauses.size() + blocks.size();

    ofstream outfile("attack1600.cnf");
    outfile << "p cnf " << total_vars << " " << total_clauses << "\n";
    
    // 1. 寫入線性層約束
    for(const auto& cls : linear_clauses) {
        for(int lit : cls) outfile << lit << " ";
        outfile << "0\n";
    }

    // 2. 寫入 S-box 約束
    for(int y=0; y<5; y++){ 
        for(int z=0; z<64; z++){ 
            for(const auto& cls : base_sbox_clauses) {
                for(int lit : cls) {
                    int s = (lit < 0) ? -1 : 1; 
                    int x = abs(lit); 
                    outfile << s * (f_idx(x) + 320*y + z) << " ";
                }
                outfile << "0\n";
            }
        }
    }
    
    // 3. 輸出 alpha_2 限制 (原 1601~3200，現為 3521~5120)
    for(int i = 0; i < 1600; i++) {
        int var_id = 3521 + i;
        if (alpha_2[i] == 1) outfile << var_id << " 0\n";
        else outfile << -var_id << " 0\n";
    }

    // 4. 輸入最前面的 Alpha 限制 (最後的 262 個要為 0，也就是 1339 ~ 1600 要是 0)
    for(int i = 1338; i < 1600; i++) {
        int var_id = 1 + i;
        outfile << -var_id << " 0\n";
    }

    // 5. 加入阻擋子句 (Blocking Clauses - 針對 1921 ~ 3520 的 Beta 段落避免重複解)
    for(const auto& b_cls : blocks) {
        for(int lit : b_cls) outfile << lit << " ";
        outfile << "0\n";
    }
    
    outfile.close();
}

// === [輔助函數] 呼叫 CryptoMiniSat 並讀取 1921~3520 (Beta) ===
// === [輔助函數] 呼叫 CryptoMiniSat 並讀取 1921~3520 (Beta) ===
bool run_sat_solver_and_get_beta0(vector<int>& beta_0) {
    cout << "  > 執行 SAT Solver (CryptoMiniSat)..." << flush;
    
    bool is_sat = false;
    int max_retries = 5; // 最多重試 5 次
    
    for (int attempt = 1; attempt <= max_retries; ++attempt) {
        // 給系統一點緩衝時間，確保上一步寫入的 attack1600.cnf 已經被解鎖 (50毫秒)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // 呼叫 SAT Solver
        system("cryptominisat5 attack1600.cnf > sat_result.txt");

        ifstream infile("sat_result.txt");
        if (!infile.is_open()) {
            cout << " [讀取結果失敗，等待重試...] " << flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue; // 重新執行 for 迴圈
        }

        string line;
        bool has_valid_output = false; 
        beta_0.assign(1600, 0);

        while (getline(infile, line)) {
            // 如果讀到 s 開頭，代表 SAT Solver 真的有成功執行並給出結論
            if (line.rfind("s ", 0) == 0) {
                has_valid_output = true;
                if (line.find("SATISFIABLE") != string::npos && line.find("UNSATISFIABLE") == string::npos) {
                    is_sat = true;
                }
            } else if (line.rfind("v ", 0) == 0) {
                stringstream ss(line.substr(2));
                int var;
                while (ss >> var) {
                    if (var == 0) continue;
                    int abs_var = abs(var);
                    if (abs_var >= 1921 && abs_var <= 3520) {
                        beta_0[abs_var - 1921] = (var > 0) ? 1 : 0;
                    }
                }
            }
        }
        
        infile.close();

        // 判斷是否是一次成功的執行 (避免 Permission denied 導致的空結果)
        if (has_valid_output) {
            cout << (is_sat ? " SATISFIABLE!" : " UNSATISFIABLE!") << "\n";
            return is_sat;
        } else {
            cout << " [檔案被鎖死或程式崩潰，0.5秒後重試...] " << flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    
    // 如果重試了 5 次還是失敗，直接終止程式，不要騙人說是 UNSAT
    cerr << "\n\n[致命錯誤] 連續 " << max_retries << " 次呼叫 SAT Solver 失敗！\n";
    cerr << "請檢查是否被 OneDrive 或防毒軟體阻擋。\n";
    system("pause");
    exit(1); 
}

// === [輔助函數] 第二個檔案 Matrix 相關 ===
void VtoM(const vector<int>& v, MatrixType2D& m){
    for(int i = 0; i < 1600; i++)m[64*(i/320)+(i%64)][(i/64)%5]=v[i];     
}

vector<uint32_t> UINT32(int i){
     uint32_t AL = Alpha_1[i][0] + Alpha_1[i][1]*2 + Alpha_1[i][2]*4 + Alpha_1[i][3]*8 + Alpha_1[i][4]*16;
     uint32_t BE = Beta_0[i][0] + Beta_0[i][1]*2 + Beta_0[i][2]*4 + Beta_0[i][3]*8 + Beta_0[i][4]*16;
     return {AL, BE};    
}


MatrixType2D DDT4(vector<vector<int>> c){
    MatrixType2D L(5, vector<int>(6, 0));
    for(int i = 0; i < 5; i++) L[i][i] = 1;    
    
    for(int i = 0; i < 5; i++){
        if(c[i][(i+1)%5]+c[i][(i+2)%5]==1){
     
            
            if(c[i][(i+1)%5]==1){
     
                if(c[i][5]==1){
                    L[(i+4)%5][i] = 1;
                    L[(i+4)%5][5] = 1;
                } else L[i][(i+2)%5] = 1;
            } else {
                if(c[i][5]==1){
                    L[i][(i+1)%5]=1;
                    L[i][5] = 1;    
                } else L[(i+1)%5][(i+3)%5]=1;
            }
        }
    }
    for(int i = 0; i < 5; i ++){
        if(c[i][(i+1)%5]+c[i][(i+2)%5]==2){
        	if(c[i][5] == 1) {
			L[i][(i+2)%5] = 1;
		}

    }
}
    return L;

}



MatrixType2D CC(uint32_t m, uint32_t n) { 
    MatrixType2D c(5, vector<int>(6, 0)); 
    int x[5], y[5];
    for(int i = 0; i < 5; i++) {
        x[i] = (m >> i) & 1;
        y[i] = (n >> i) & 1;
    }
    for (int i = 0; i < 5; i++) {
        int c2_i = x[i] ^ ((x[(i + 1) % 5] ^ 1) * x[(i + 2) % 5]);
        c[i][5] = y[i] ^ c2_i;
    }
    c[0][1] = x[2]; c[0][2] = x[1];
    c[1][2] = x[3]; c[1][3] = x[2];
    c[2][3] = x[4]; c[2][4] = x[3];
    c[3][0] = x[4]; c[3][4] = x[0];
    c[4][0] = x[1]; c[4][1] = x[0];
    return c;
}

MatrixType2D DDT2or4(uint32_t m, uint32_t n) { return DDT4(CC(m, n)); }

bool Gaussian(MatrixType2D D){
    int rows = D.size(), cols = D[0].size(), pivot_row = 0;
    for (int c = 0; c < cols - 1 && pivot_row < rows; c++) {
        int sel = pivot_row;
        while (sel < rows && D[sel][c] == 0) sel++;
        if (sel == rows) continue;
        if (sel != pivot_row) swap(D[pivot_row], D[sel]);

        for (int i = 0; i < rows; i++) {
            if (i != pivot_row && D[i][c] == 1) {
                for (int k = c; k < cols; k++) D[i][k] ^= D[pivot_row][k]; 
            }
        }
        pivot_row++;
    }
    for (int i = pivot_row; i < rows; i++) {
        if (D[i][cols - 1] == 1) return false; 
    }
    return true; 
}

MatrixType2D DDT8(uint32_t m, uint32_t n, vector<bool>& is_nonlinear, int sbox_index) {
    MatrixType2D c = CC(m, n); 
    MatrixType2D L(5, vector<int>(6, 0)); 
    for(int j = 0; j < 5; j++){ 
        L[j][j]=1;   
        if(c[j][(j+1)%5] + c[j][(j+2)%5] == 0 && c[(j+4)%5][(j+1)%5] == 0 && c[(j+1)%5][(j+2)%5] == 0) {
            is_nonlinear[320*(sbox_index/64) + (sbox_index % 64) + 64*j] = true; 
            L[j][j] = 0;
        }
    }
    for(int i = 0; i < 5; i++){
        int sum_c = c[i][(i+1)%5] + c[i][(i+2)%5];
        if(sum_c == 1){
            if(c[i][(i+1)%5] == 1){
       
                if(c[i][5] == 1){
                    L[(i+4)%5][i] = 1;
                    L[(i+4)%5][5] = 1;
                } else L[i][(i+2)%5] = 1;
            } else {
  
                if(c[i][5] == 1){
                    L[i][(i+1)%5] = 1;
                    L[i][5] = 1;  
                } else L[(i+1)%5][(i+3)%5] = 1;
            }
        } else if(sum_c == 2) {
            if(c[i][5] == 1) L[i][(i+2)%5] = 1;
        }
    }
    return L;
}

MatrixType2D Substitution(const MatrixType2D& M, const MatrixType2D& N, const vector<bool>& is_nonlinear) {
    MatrixType2D m(M.size(), vector<int>(M[0].size(), 0));
    for (size_t i = 0; i < M.size(); i++) {
        bool equation_is_poisoned = false;
        for (int j = 0; j < 1600; j++) {
            if (M[i][j] == 1 && is_nonlinear[j]) {
                equation_is_poisoned = true;
                break; 
            }
        }
        if (equation_is_poisoned) continue;
        for (int j = 0; j < 1600; j++) { 
            if (M[i][j] == 1) {
                for (int k = 0; k < 1601; k++) m[i][k] ^= N[j][k];
            }
        }
        m[i][1600] ^= M[i][1600]; 
    }
    return m;
}

MatrixType2D Merge(const MatrixType2D& M, const MatrixType2D& N){
	MatrixType2D m(M.size() + N.size(), vector<int>(M[0].size()));
	for(size_t i = 0; i < M.size(); i++) m[i] = M[i];
	for(size_t i = M.size(); i < M.size() + N.size(); i++) m[i] = N[i - M.size()];
	return m;	
}


// === [主程式] ===
int main() {
    // 預先計算基礎 S-box 與 線性層 的 CNF 約束
    precompute_base_sbox_constraints();
    precompute_linear_layer_clauses(); // 新增的線性層生成

    // 1. 讀取矩陣檔案
    int rows = 262, cols = 1601;
    MatrixType2D E_M(rows, vector<int>(cols));
    ifstream inFile("E_M.txt");
    if (!inFile) { cerr << "找不到 E_M.txt！\n"; return 1; }
    for (int i = 0; i < rows; ++i) for (int j = 0; j < cols; ++j) inFile >> E_M[i][j];
    inFile.close();

    int rows_1 = 1600, cols_1 = 1601;
    MatrixType2D D(rows_1, vector<int>(cols_1));
    ifstream inFileD("D.txt");
    if (!inFileD) { cerr << "找不到 D.txt！\n"; return 1; }
    for (int i = 0; i < rows_1; ++i) for (int j = 0; j < cols_1; ++j) inFileD >> D[i][j];
    inFileD.close();
    
    
        cout << endl;

    VtoM(alpha_1, Alpha_1); // alpha_1 是固定的

    bool success = false;
    int MAX_ATTEMPTS = 10;
    
    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        cout << "\n========== 第 " << attempt << " 次測試 ==========\n";
        
        // 2. 產出 CNF 檔案 (帶有之前失敗的阻擋條件，且已經加上線性層)
        generate_attack1600_cnf(blocking_clauses);

        // 3. 執行求解並獲取 Beta_0 (這裡取的是平移後的 1921~3520)
        vector<int> current_beta_0;
        bool is_sat = run_sat_solver_and_get_beta0(current_beta_0);
        
        if (!is_sat) {
            cout << "SAT Solver 回傳 UNSAT。表示所有的可能性都被阻擋完了，無解！\n";
            break; 
        }


        // 4. 重置與矩陣前置處理
        vector<bool> is_nonlinear(1600, false);
        VtoM(current_beta_0, Beta_0);
        MatrixType2D L1600(1600, vector<int>(1601, 0));
        
              MatrixType2D C(1600, vector<int>(1601, 0));
        
        for(int i = 0; i < 320; i++){
        	vector<uint32_t> UU = UINT32(i);
        	MatrixType2D C_1(5, vector<int>(6, 0));
        	C_1 = CC(UU[1], UU[0]);
        	
        	for(int j = 0; j < 5; j++) for(int k = 0; k < 5; k++) C[320*(i/64)+(i%64)+64*j][320*(i/64)+(i%64)+64*k] = C_1[j][k];
            for(int j = 0; j < 5; j++) C[320*(i/64)+(i%64)+64*j][1600] = C_1[j][5];	
		}

        // 5. 計算 DDT 與 S-box 行為
        
        bool invalid_ddt = false;
        for(int i = 0; i < 320; i++){
            vector<uint32_t> UU = UINT32(i);
            int DDT = matrix_lookup[UU[1]][UU[0]];
            MatrixType2D L(5, vector<int>(6, 0));
          
            if(DDT == 2 || DDT == 4) {
            //	for(int j = 0; j < 5; j++)
            //	is_nonlinear[320*(i/64) + (i% 64) + 64*j] = true;
			L = DDT2or4(UU[1], UU[0]);
		}
            else if(DDT == 8)    {
			    L = DDT8(UU[1], UU[0], is_nonlinear, i);
			}
            else if(DDT == 32) {
            	for(int j = 0; j < 5; j++)
            	is_nonlinear[320*(i/64) + (i% 64) + 64*j] = true;
           
            } else {
                cout << "  > S-box [" << i << "] 找到無效的 DDT 值 (" << DDT << ")\n";
                invalid_ddt = true;
                break;
            }
            
            for(int j = 0; j < 5; j++) for(int k = 0; k < 5; k++) L1600[320*(i/64)+(i%64)+64*j][320*(i/64)+(i%64)+64*k] = L[j][k];
            for(int j = 0; j < 5; j++) L1600[320*(i/64)+(i%64)+64*j][1600] = L[j][5];
        }

        if (invalid_ddt) {
            cout << "  > DDT 值不合理，準備加入 Blocking Clause 重新尋找...\n";
            // 下方會自動加入 blocking_clause
        } else {
            // 6. 矩陣代入與高斯消去檢驗
            MatrixType2D E_M_1 = Substitution(D, L1600, is_nonlinear);
            MatrixType2D E_M_temp = Merge(E_M_1, E_M);
             MatrixType2D E_M_temp_1 = Merge(E_M_1, C);
            
            if(Gaussian(E_M_temp_1)) {
                cout << "  > Gaussian 消去成功！找到符合條件的 beta_0 了！\n";
                E_M = E_M_temp_1;
                success = true;
                break; // 測試成功，跳出迴圈
            } else {
                cout << "  > Gaussian 消去產生矛盾 (0=1)，失敗。\n";

            }
        }

        // 7. 如果失敗，將這組 current_beta_0 轉換成 Blocking Clause (De Morgan's Law)
        // 注意：這裡因為我們綁定的是 Beta_0，對應的變數是 1921 ~ 3520
        vector<int> b_clause;
        for(int i = 0; i < 1600; i++) {
            int var_id = 1921 + i; 
            if (current_beta_0[i] == 1) b_clause.push_back(-var_id); 
            else b_clause.push_back(var_id);                         
        }
        blocking_clauses.push_back(b_clause);
        cout << "  > 已將此解加入 Blocking Clause 排除，進行下一輪...\n";
    }

    if (success) {
        ofstream outEM("Updated_E_M.txt");
        for(size_t i = 0; i < E_M.size(); i++){
            for(size_t j = 0; j < E_M[i].size(); j++) {
                outEM << E_M[i][j] << (j == E_M[i].size() - 1 ? "" : " ");
            }
            outEM << "\n";
        }
        outEM.close();
        cout << "\n?? 程式執行成功！更新後的 E_M 矩陣已輸出至 Updated_E_M.txt (總列數: " << E_M.size() << ")\n";
    } else {
        cout << "\n? 跑了 " << MAX_ATTEMPTS << " 次依然沒有找到符合 Gaussian 條件的解。\n";
    }
    
    return 0;
}
