#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>

using namespace std;

typedef uint64_t u64;
typedef vector<vector<int>> Matrix2D;

const int r_ro[5][5] = {
    { 0, 36,  3, 41, 18},
    { 1, 44, 10, 45,  2},
    {62,  6, 43, 15, 61},
    {28, 55, 25, 21, 56},
    {27, 20, 39,  8, 14}
};

// ==========================================
// 3. 標準 FIPS 202 正向運算 (用來建立矩陣)
// ==========================================
void theta(int in[5][5][64], int out[5][5][64]) {
    int C[5][64] = {0}, D[5][64] = {0};
    for(int x=0; x<5; x++)
        for(int z=0; z<64; z++)
            C[x][z] = in[x][0][z] ^ in[x][1][z] ^ in[x][2][z] ^ in[x][3][z] ^ in[x][4][z];

    for(int x=0; x<5; x++)
        for(int z=0; z<64; z++)
            D[x][z] = C[(x+4)%5][z] ^ C[(x+1)%5][(z+63)%64];

    for(int x=0; x<5; x++)
        for(int y=0; y<5; y++)
            for(int z=0; z<64; z++)
                out[x][y][z] = in[x][y][z] ^ D[x][z];
}

void rho(int in[5][5][64], int out[5][5][64]) {
    for(int x=0; x<5; x++)
        for(int y=0; y<5; y++)
            for(int z=0; z<64; z++) {
                int r = r_ro[x][y];
                out[x][y][z] = in[x][y][(z-r+64)%64];
            }
}

void pi(int in[5][5][64], int out[5][5][64]) {
    for(int x=0; x<5; x++)
        for(int y=0; y<5; y++)
            for(int z=0; z<64; z++) {
                out[y][(2*x + 3*y)%5][z] = in[x][y][z];
            }
}

// ==========================================
// 4. 建構「完全大 L 矩陣」並求反矩陣
// ==========================================
Matrix2D getFullLInverseMatrix() {
    Matrix2D L_matrix(1600, vector<int>(1600, 0));
    Matrix2D C_aug(1600, vector<int>(3200, 0));
    Matrix2D inv_L(1600, vector<int>(1600, 0));

    // 步驟 A: 生成 1600x1600 的大 L 矩陣 (Theta -> Rho -> Pi)
    for(int i = 0; i < 1600; i++) {
        int S0[5][5][64] = {0}, S1[5][5][64] = {0}, S2[5][5][64] = {0}, S3[5][5][64] = {0};
        
        // 設定第 i 個 bit 為 1
        int y_in = i / 320, x_in = (i / 64) % 5, z_in = i % 64;
        S0[x_in][y_in][z_in] = 1;

        // 通過標準線性層
        theta(S0, S1);
        rho(S1, S2);
        pi(S2, S3);

        // 將輸出的 3D 陣列攤平成矩陣的第 i 個 Column
        for(int y = 0; y < 5; y++) {
            for(int x = 0; x < 5; x++) {
                for(int z = 0; z < 64; z++) {
                    int out_idx = 320 * y + 64 * x + z;
                    L_matrix[out_idx][i] = S3[x][y][z];
                }
            }
        }
    }

    // 步驟 B: 設定增廣矩陣 [L | I]
    for (int i = 0; i < 1600; i++) {
        for (int j = 0; j < 1600; j++) C_aug[i][j] = L_matrix[i][j];
        C_aug[i][i + 1600] = 1;
    }

    // 步驟 C: 高斯消去法求反矩陣
    for(int i = 0; i < 1600; i++) {
        int pivot = i;
        while (pivot < 1600 && C_aug[pivot][i] == 0) pivot++;
        if (pivot == 1600) { cerr << "不可逆!" << endl; return inv_L; }
        if (pivot != i) swap(C_aug[i], C_aug[pivot]);
        for(int r = 0; r < 1600; r++) {
            if(r != i && C_aug[r][i] == 1) {
                for(int c = i; c < 3200; c++) C_aug[r][c] ^= C_aug[i][c];
            }
        }
    }

    // 步驟 D: 擷取右半部
    for (int i = 0; i < 1600; i++) {
        for (int j = 0; j < 1600; j++) inv_L[i][j] = C_aug[i][j + 1600];
    }
    return inv_L;
}

// ==========================================
// 5. 矩陣乘法與主程式
// ==========================================
vector<int> multiplyMatrixVectorGF2(const Matrix2D& matrix, const vector<int>& vec) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<int> result(rows, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i] ^= (matrix[i][j] & vec[j]);
        }
    }
    return result;
}

int main() {
    // 建立一個 ofstream 物件，準備寫入 "data.txt"
    ofstream outFile("E_M.txt");

    // 檢查檔案是否成功建立
    if (!outFile) {
        cerr << "無法建立檔案！\n";
        return 1;
    }
    
    Matrix2D fullInverseMatrix = getFullLInverseMatrix();
    Matrix2D E_M(1600,vector<int>(1601,0));
    
    for(int i = 0; i < 1600; i++)
    for(int j = 0; j < 1600; j++)
    E_M[i][j]=fullInverseMatrix[i][j];
    
    for(int i = 1344; i < 1600; i++)
    E_M[i][1600]=0;
    for(int i = 1338; i < 1344; i++)
    E_M[i][1600]=1;
   
    for(int i = 1338; i < 1600; i++){
    	for(int j = 0; j < 1601; j++){
    		
    		outFile << E_M[i][j]<<" ";
		}
		outFile << "\n";
		
	}

    outFile.close();
    
     cout << "第一支程式執行完畢！資料已成功輸出到 E_M.txt\n";
    


    return 0;
}
