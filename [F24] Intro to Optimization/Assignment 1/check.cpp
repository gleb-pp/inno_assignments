#include <iostream>
#include <vector>
#include <iomanip>

#define DEFAULT_ACCURACY 3

using namespace std;

int vars = objFun.size();         // number of variables
int n = constrFun.size();         // number of equations (exluding the objective)
bool solvable = true;             // does function have a solution
vector<double> pivots(vars, 0);   // pivots[variable_index] = index of its line

// creating a table
vector<vector<double>> table(n + 1, vector<double>(vars + n + 2, 0));

vector<vector<double>> table = {
    {-704, -401, 100, 0, 0, 0},

}

while (true) {

    // findind key column
    int kk = findKeyColumn(table, vars + n);
    if (kk == -1) {
        break;
    }

    // computing Ratio
    for (int i = 0; i < n + 1; i++) {
        if (table[i][kk] == 0) {
            table[i][vars + n + 1] = -1;
        } else {
            table[i][vars + n + 1] = table[i][vars + n] / table[i][kk];
        }
    }

    // finding key row
    int kr = findKeyRow(table, n, vars);
    if (kr == -1) {
        solvable = false;
        break;
    }

    // dividing the key row by key element
    double keyelem = table[kr][kk];
    for (int i = 0; i < vars + n + 1; i++) {
        table[kr][i] = table[kr][i] / keyelem;
    }

    // creating new table
    for (int i = 0; i < n + 1; i++) {
        keyelem = table[i][kk];
        if (keyelem == table[kr][kk]) {
            continue;
        }
        for (int j = 0; j < vars + n + 1; j++) {
            table[i][j] = table[i][j] - (keyelem * table[kr][j] / table[kr][kk]);
        }
    }
    pivots[kk] = kr;
}

// transferring data from a table to a result
// if (i = 0), leave it 0, if not, replace with the RHS of its line
for (int i = 0; i < vars; i++) {
    if (pivots[i] != 0) {
        pivots[i] = table[pivots[i]][vars + n];
    }
}