#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;
ifstream inputFile("input.txt");
#define GNUPLOT_NAME "C:\\gnuplot\\bin\\gnuplot -persist"
FILE* pipe = _popen(GNUPLOT_NAME, "w");

void build(const int& N, const vector<double>& x, const vector<double>& y) {
    fprintf(pipe, "%s\n", "set xrange [-1:35]");
    fprintf(pipe, "%s\n", "set yrange [-1:100]");
    fprintf(pipe, "%s\n", "plot '-' using 1:2 title 'exp' with points");

    for (int i = 0; i < N; i++)
        fprintf(pipe, "%f\t%f\n", x[i], y[i]);

    fprintf(pipe, "%s\n", "e");
    fflush(pipe);
    _pclose(pipe);
}

int main() {
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    int m;
    inputFile >> m;
    vector<double> pointX, pointY; // Изменили на double
    for (int i = 0; i < m; i++) {
        double x, y; // Изменили на double
        inputFile >> x >> y;
        pointX.push_back(x);
        pointY.push_back(y);
    }
    int k;
    inputFile >> k;
    build(m, pointX, pointY);

    inputFile.close(); // Закрываем файл после чтения
}
