#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Matrix {
protected:
    vector<vector<double>> matrix;
    int rows;
    int columns;

public:
    Matrix(int r, int c) {
        rows = r;
        columns = c;
        matrix = vector<vector<double>>(rows, vector<double>(columns, 0));
    }

    friend istream& operator>>(istream& in, Matrix& mat) {
        for (int i = 0; i < mat.rows; i++) {
            for (int j = 0; j < mat.columns; j++) {
                in >> mat.matrix[i][j];
            }
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, const Matrix& mat) {
        for (int i = 0; i < mat.rows; ++i) {
            for (int j = 0; j < mat.columns; ++j) {
                out << fixed << setprecision(2) << mat.matrix[i][j] << " ";
            }
            out << endl;
        }
        return out;
    }

    virtual void operator=(const Matrix& other) {
        rows = other.rows;
        columns = other.columns;
        matrix = vector<vector<double>>(rows, vector<double>(columns, 0));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    virtual Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || columns != other.columns) {
            cout << "err: the dimensional problem occurred" << endl;
            return Matrix(0, 0);
        } else {
            Matrix result(rows, columns);
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < columns; j++) {
                    result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
                }
            }
            return result;
        }
    }

    virtual Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || columns != other.columns) {
            cout << "err: the dimensional problem occurred" << endl;
            return Matrix(0, 0);
        } else {
            Matrix result(rows, columns);
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < columns; j++) {
                    result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
                }
            }
            return result;
        }
    }

    virtual Matrix operator*(const Matrix& other) const {
        if (columns != other.rows) {
            cout << "err: the dimensional problem occurred" << endl;
            return Matrix(0, 0);
        } else {
            Matrix result(rows, other.columns);
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < other.columns; j++) {
                    result.matrix[i][j] = 0;
                    for(int k = 0; k < columns; k++) {
                        result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                    }
                }
            }
            return result;
        }
    }

    virtual Matrix transpose() const {
        Matrix result(columns, rows);
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                result.matrix[j][i] = matrix[i][j];
            }
        }
        return result;
    }

    int getRows() const {
        return rows;
    }

    int getColumns() const {
        return columns;
    }

    auto getMatrix() const {
        return matrix;
    }

    void setElem(int i, int j, double elem) {
        matrix[i][j] = elem;
    }

};

class EliminationMatrix : public Matrix {
public:
    EliminationMatrix(int i, int j, Matrix A) : Matrix(A.getRows(), A.getColumns()) {
        if (i > 0 && i <= A.getRows() && j > 0 && j <= A.getColumns()) {
            if (A.getMatrix()[i - 1][j - 1] != 0) {
                for (int k = 0; k < (A.getRows() - 1); k++) {
                    double elem = A.getMatrix()[(i + k) % A.getRows()][j - 1];
                    bool flag = false;
                    if (elem < -0.00001 || elem > 0.00001) {
                        for (int a = 0; a < rows; a++) {
                            for (int b = 0; b < columns; b++) {
                                if (a == (i-1) && b == ((i + k) % A.getRows())) {
                                    matrix[a][b] = - (A.getMatrix()[i-1][j-1] / elem);
                                    flag = true;
                                    if (i > j) {
                                        for (int ch = j; ch >= 0; ch--) {
                                            if (A.getMatrix()[(i + k) % A.getRows()][ch] != 0) {
                                                flag = false;
                                                break;
                                            }
                                        }
                                    } else {
                                        for (int ch = j; ch < rows; ch++) {
                                            if (A.getMatrix()[(i + k) % A.getRows()][ch] != 0) {
                                                flag = false;
                                                break;
                                            }
                                        }
                                    }
                                } else if (a == b) {
                                    matrix[a][b] = 1;
                                } else {
                                    matrix[a][b] = 0;
                                }
                            }
                        }
                        if (flag) {
                            break;
                        }
                    }
                }
            } else {
                for (int a = 0; a < rows; a++) {
                    for (int b = 0; b < columns; b++) {
                        matrix[a][b] = (double)(a == b);
                    }
                }
            }
        } else {
            cout << "err: the dimensional problem occurred" << endl;
        }
    }
};

class PermutationMatrix : public Matrix {
public:
    PermutationMatrix(int i1, int i2, Matrix A) : Matrix(A.getRows(), A.getColumns()) {
        i1--;
        i2--;
        for (int a = 0; a < rows; a++) {
            for (int b = 0; b < columns; b++) {
                if (a == i1) {
                    if (b == i2) {
                        matrix[a][b] = 1;
                    } else {
                        matrix[a][b] = 0;
                    }
                } else if (a == i2) {
                    if (b == i1) {
                        matrix[a][b] = 1;
                    } else {
                        matrix[a][b] = 0;
                    }
                } else {
                    if (a == b) {
                        matrix[a][b] = 1;
                    } else {
                        matrix[a][b] = 0;
                    }
                }
            }
        }
    }
};

class IdentityMatrix : public Matrix {
public:
    IdentityMatrix(int size) : Matrix(size, size) {
        for (int i = 0; i < size; ++i) {
            matrix[i][i] = 1;
        }
    }
};

class ColumnVector : public Matrix {
public:
    ColumnVector(int size) : Matrix(size, 1) {}

    ColumnVector(const Matrix& other) : Matrix(other.getRows(), 1) {
        if (other.getColumns() != 1) {
            cout << "err: the dimensional problem occurred" << endl;
            return;
        }
        for (int i = 0; i < rows; i++) {
            matrix[i][0] = other.getMatrix()[i][0];
        }
    }

    void operator=(const Matrix& other) override {
        if (other.getRows() != rows || other.getColumns() != 1) {
            cout << "err: the dimensional problem occurred" << endl;
            return;
        }
        Matrix::operator=(other);
    }

    Matrix operator+(const Matrix& other) const override {
        if (rows != other.getRows() || other.getColumns() != 1) {
            cout << "err: the dimensional problem occurred" << endl;
            return ColumnVector(0);
        }
        ColumnVector result(rows);
        for (int i = 0; i < rows; i++) {
            result.matrix[i][0] = matrix[i][0] + other.getMatrix()[i][0];
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const override {
        if (rows != other.getRows() || other.getColumns() != 1) {
            cout << "err: the dimensional problem occurred" << endl;
            return ColumnVector(0);
        }
        ColumnVector result(rows);
        for (int i = 0; i < rows; i++) {
            result.matrix[i][0] = matrix[i][0] - other.getMatrix()[i][0];
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const override {
        if (other.getColumns() != 1) {
            cout << "err: the dimensional problem occurred" << endl;
            return ColumnVector(0);
        }
        ColumnVector result(rows);
        for (int i = 0; i < rows; i++) {
            result.matrix[i][0] = 0;
            for (int j = 0; j < columns; j++) {
                result.matrix[i][0] += matrix[i][j] * other.getMatrix()[j][0];
            }
        }
        return result;
    }
};

class SquareMatrix : public Matrix {
public:
    SquareMatrix(int size) : Matrix(size, size) {}

    SquareMatrix(const Matrix& other) : Matrix(other.getRows(), other.getColumns()) {
        if (other.getRows() != other.getColumns()) {
            cout << "err: the dimensional problem occurred" << endl;
            return;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = other.getMatrix()[i][j];
            }
        }
    }

    void operator=(const Matrix& other) override {
        if (other.getRows() != rows || other.getColumns() != columns) {
            cout << "err: the dimensional problem occurred" << endl;
            return;
        }
        Matrix::operator=(other);
    }

    Matrix operator+(const Matrix& other) const override {
        if (rows != other.getRows() || columns != other.getColumns()) {
            cout << "err: the dimensional problem occurred" << endl;
            return SquareMatrix(0);
        }
        SquareMatrix result(rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result.matrix[i][j] = matrix[i][j] + other.getMatrix()[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const override {
        if (rows != other.getRows() || columns != other.getColumns()) {
            cout << "err: the dimensional problem occurred" << endl;
            return SquareMatrix(0);
        }
        SquareMatrix result(rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result.matrix[i][j] = matrix[i][j] - other.getMatrix()[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const override {
        if (columns != other.getRows()) {
            cout << "err: the dimensional problem occurred" << endl;
            return SquareMatrix(0);
        }
        SquareMatrix result(rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.getColumns(); j++) {
                result.matrix[i][j] = 0;
                for (int k = 0; k < columns; k++) {
                    result.matrix[i][j] += matrix[i][k] * other.getMatrix()[k][j];
                }
            }
        }
        return result;
    }

    Matrix transpose() const override {
        SquareMatrix result(rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result.matrix[j][i] = matrix[i][j];
            }
        }
        return result;
    }

    double determinant() const {
        SquareMatrix A = *this;
        double det = 1.0;
        int step = 1;

        for (int i = 0; i < rows - 1; i++) {

            // searching for maximum element
            double maxx = A.getMatrix()[i][i];
            maxx = (maxx > 0) ? maxx : -maxx;
            int maxRow = i;
            for (int j = i + 1; j < rows; j++) {
                double check = A.getMatrix()[j][i];
                check = (check > 0) ? check : -check;
                if (check > maxx) {
                    maxx = check;
                    maxRow = j;
                }
            }
            if (maxRow != i) {
                PermutationMatrix P(i + 1, maxRow + 1, A);
                A = P * A;

                cout << "step #" << step++ << ": permutation" << endl;
                cout << A;
            }

            // eliminations
            for (int j = i + 1; j < rows; j++) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;

                    cout << "step #" << step++ << ": elimination" << endl;
                    cout << A;
                }
            }
            det *= A.getMatrix()[i][i];
        }
        return det * A.getMatrix()[rows - 1][rows - 1];
    }

    Matrix inversed() const {
        SquareMatrix A = *this;
        SquareMatrix B = IdentityMatrix(rows);
        if (A.determinant() == 0) {
            cout << "err: matrix A is singular" << endl;
            exit(1);
        }
        cout << "Augmented matrix:" << endl;
        for (int a = 0; a < rows; a++) {
            for (int b = 0; b < rows; b++) {
                cout << fixed << setprecision(2) << A.getMatrix()[a][b] << " ";
            }
            for (int b = 0; b < rows; b++) {
                cout << fixed << setprecision(2) << B.getMatrix()[a][b] << " ";
            }
            cout << endl;
        }

        cout << "Gaussian process:" << endl;
        int step = 1;
        for (int i = 0; i < rows - 1; i++) {

            // searching for maximum element
            double maxx = A.getMatrix()[i][i];
            maxx = (maxx > 0) ? maxx : -maxx;
            int maxRow = i;
            for (int j = i + 1; j < rows; j++) {
                double check = A.getMatrix()[j][i];
                check = (check > 0) ? check : -check;
                if (check > maxx) {
                    maxx = check;
                    maxRow = j;
                }
            }
            if (maxRow != i) {
                PermutationMatrix P(i + 1, maxRow + 1, A);
                A = P * A;
                B = P * B;

                cout << "step #" << step++ << ": permutation" << endl;
                for (int a = 0; a < rows; a++) {
                    for (int b = 0; b < rows; b++) {
                        cout << fixed << setprecision(2) << A.getMatrix()[a][b] << " ";
                    }
                    for (int b = 0; b < rows; b++) {
                        cout << fixed << setprecision(2) << B.getMatrix()[a][b] << " ";
                    }
                    cout << endl;
                }
            }

            // eliminations
            for (int j = i + 1; j < rows; j++) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    cout << "step #" << step++ << ": elimination" << endl;
                    for (int a = 0; a < rows; a++) {
                        for (int b = 0; b < rows; b++) {
                            cout << fixed << setprecision(2) << A.getMatrix()[a][b] << " ";
                        }
                        for (int b = 0; b < rows; b++) {
                            cout << fixed << setprecision(2) << B.getMatrix()[a][b] << " ";
                        }
                        cout << endl;
                    }
                }
            }
        }

        for (int i = rows - 1; i > 0; i--) {
            for (int j = i - 1; j >= 0; j--) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    cout << "step #" << step++ << ": elimination" << endl;
                    for (int a = 0; a < rows; a++) {
                        for (int b = 0; b < rows; b++) {
                            cout << fixed << setprecision(2) << A.getMatrix()[a][b] << " ";
                        }
                        for (int b = 0; b < rows; b++) {
                            cout << fixed << setprecision(2) << B.getMatrix()[a][b] << " ";
                        }
                        cout << endl;
                    }
                }
            }
        }

        cout << "Diagonal normalization:" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                B.setElem(i, j, B.getMatrix()[i][j] / A.getMatrix()[i][i]);
            }
            A.setElem(i, i, 1);
        }

        for (int a = 0; a < rows; a++) {
            for (int b = 0; b < rows; b++) {
                cout << fixed << setprecision(2) << A.getMatrix()[a][b] << " ";
            }
            for (int b = 0; b < rows; b++) {
                cout << fixed << setprecision(2) << B.getMatrix()[a][b] << " ";
            }
            cout << endl;
        }
        return B;
    }

    void SLAE(ColumnVector B) const {
        SquareMatrix A = *this;
        cout << "Gaussian process:" << endl;
        int step = 1;
        for (int i = 0; i < rows - 1; i++) {

            // searching for maximum element
            double maxx = A.getMatrix()[i][i];
            maxx = (maxx > 0) ? maxx : -maxx;
            int maxRow = i;
            for (int j = i + 1; j < rows; j++) {
                double check = A.getMatrix()[j][i];
                check = (check > 0) ? check : -check;
                if (check > maxx) {
                    maxx = check;
                    maxRow = j;
                }
            }
            if (maxRow != i) {
                PermutationMatrix P(i + 1, maxRow + 1, A);
                A = P * A;
                B = P * B;

                cout << "step #" << step++ << ": permutation" << endl;
                cout << A;
                cout << B;
            }

            // eliminations
            for (int j = i + 1; j < rows; j++) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    cout << "step #" << step++ << ": elimination" << endl;
                    cout << A;
                    cout << B;
                }
            }
        }

        for (int i = rows - 1; i > 0; i--) {
            for (int j = i - 1; j >= 0; j--) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    cout << "step #" << step++ << ": elimination" << endl;
                    cout << A;
                    cout << B;
                }
            }
        }

        cout << "Diagonal normalization:" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                B.setElem(i, j, B.getMatrix()[i][j] / A.getMatrix()[i][i]);
            }
            A.setElem(i, i, 1);
        }

        cout << A;
        cout << B;
        cout << "Result:" << endl << B;
        exit(0);
        return;
    }
};

int main() {
    int sA;
    cin >> sA;
    SquareMatrix A(sA);
    cin >> A;

    int nCV;
    cin >> nCV;
    ColumnVector b(nCV);
    cin >> b;

    double eps;
    cin >> eps;

    ColumnVector x0 = b;

    SquareMatrix D_inv(sA);
    for (int i = 0; i < sA; i++) {
        for (int j = 0; j < sA; j++) {
            if (i == j) {
                double diag = A.getMatrix()[i][j];
                if (diag == 0) {
                    cout << "The method is not applicable" << endl;
                    return 0;
                }
                D_inv.setElem(i, j, 1.0 / diag);
            }
        }
    }

    SquareMatrix LU(sA);
    for (int i = 0; i < sA; i++) {
        for (int j = 0; j < sA; j++) {
            if (i != j) LU.setElem(i, j, -A.getMatrix()[i][j]);
        }
    }

    SquareMatrix alpha = D_inv * LU;
    ColumnVector beta = D_inv * b;

    ColumnVector oldX = x0;
    int step = 0;
    while (true) {
        step += 1;
        ColumnVector newX = alpha * oldX + beta;
        double maxError = 0;
        for (int i = 0; i < sA; i++) {
            double err = newX.getMatrix()[i][0] - oldX.getMatrix()[i][0];
            if (err > maxError) {
                maxError = err;
            } else if (-err > maxError) {
                maxError = -err;
            }
        }
        cout << "x(" << step << "):" << endl;
        cout << newX << endl;
        cout << "e:" << fixed << setprecision(4) << maxError << endl;

        if (maxError <= eps) {
            break;
        }

        oldX = newX;
    }

    cout << "x~:" << endl;
    cout << oldX << endl;

    return 0;
}