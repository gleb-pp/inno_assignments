#include <iostream>
#include <vector>
#include <iomanip>

class Matrix {
protected:
    std::vector<std::vector<double>> matrix;
    int rows;
    int columns;

public:
    Matrix(int r, int c) {
        rows = r;
        columns = c;
        matrix = std::vector<std::vector<double>>(rows, std::vector<double>(columns, 0));
    }

    friend std::istream& operator>>(std::istream& in, Matrix& mat) {
        for (int i = 0; i < mat.rows; i++) {
            for (int j = 0; j < mat.columns; j++) {
                in >> mat.matrix[i][j];
            }
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
        for (int i = 0; i < mat.rows; ++i) {
            for (int j = 0; j < mat.columns; ++j) {
                out << std::fixed << std::setprecision(2) << mat.matrix[i][j] << " ";
            }
            out << std::endl;
        }
        return out;
    }

    virtual void operator=(const Matrix& other) {
        rows = other.rows;
        columns = other.columns;
        matrix = std::vector<std::vector<double>>(rows, std::vector<double>(columns, 0));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    virtual Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || columns != other.columns) {
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
            return;
        }
        for (int i = 0; i < rows; i++) {
            matrix[i][0] = other.getMatrix()[i][0];
        }
    }

    void operator=(const Matrix& other) override {
        if (other.getRows() != rows || other.getColumns() != 1) {
            std::cout << "Error: the dimensional problem occurred" << std::endl;
            return;
        }
        Matrix::operator=(other);
    }

    Matrix operator+(const Matrix& other) const override {
        if (rows != other.getRows() || other.getColumns() != 1) {
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
            return ColumnVector(0);
        }
        ColumnVector result(rows);
        for (int i = 0; i < rows; i++) {
            result.matrix[i][0] = matrix[i][0] - other.getMatrix()[i][0];
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const override {
        if (other.getRows() != 1) {
            std::cout << "Error: the dimensional problem occurred" << std::endl;
            return ColumnVector(0);
        }
        Matrix result(rows, other.getColumns());
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.getColumns(); j++) {
                result.setElem(i, j, 0);
                for (int k = 0; k < columns; k++) {
                    result.setElem(i, j, result.getMatrix()[i][j] + result.getMatrix()[i][k] * other.getMatrix()[k][j]);
                }
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
            return;
        }
        Matrix::operator=(other);
    }

    Matrix operator+(const Matrix& other) const override {
        if (rows != other.getRows() || columns != other.getColumns()) {
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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
            std::cout << "Error: the dimensional problem occurred" << std::endl;
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

                std::cout << "step #" << step++ << ": permutation" << std::endl;
                std::cout << A;
            }

            // eliminations
            for (int j = i + 1; j < rows; j++) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;

                    std::cout << "step #" << step++ << ": elimination" << std::endl;
                    std::cout << A;
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
            std::cout << "Error: matrix A is singular" << std::endl;
            exit(1);
        }
        std::cout << "Augmented matrix:" << std::endl;
        for (int a = 0; a < rows; a++) {
            for (int b = 0; b < rows; b++) {
                std::cout << std::fixed << std::setprecision(2) << A.getMatrix()[a][b] << " ";
            }
            for (int b = 0; b < rows; b++) {
                std::cout << std::fixed << std::setprecision(2) << B.getMatrix()[a][b] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Gaussian process:" << std::endl;
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

                std::cout << "step #" << step++ << ": permutation" << std::endl;
                for (int a = 0; a < rows; a++) {
                    for (int b = 0; b < rows; b++) {
                        std::cout << std::fixed << std::setprecision(2) << A.getMatrix()[a][b] << " ";
                    }
                    for (int b = 0; b < rows; b++) {
                        std::cout << std::fixed << std::setprecision(2) << B.getMatrix()[a][b] << " ";
                    }
                    std::cout << std::endl;
                }
            }

            // eliminations
            for (int j = i + 1; j < rows; j++) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    std::cout << "step #" << step++ << ": elimination" << std::endl;
                    for (int a = 0; a < rows; a++) {
                        for (int b = 0; b < rows; b++) {
                            std::cout << std::fixed << std::setprecision(2) << A.getMatrix()[a][b] << " ";
                        }
                        for (int b = 0; b < rows; b++) {
                            std::cout << std::fixed << std::setprecision(2) << B.getMatrix()[a][b] << " ";
                        }
                        std::cout << std::endl;
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

                    std::cout << "step #" << step++ << ": elimination" << std::endl;
                    for (int a = 0; a < rows; a++) {
                        for (int b = 0; b < rows; b++) {
                            std::cout << std::fixed << std::setprecision(2) << A.getMatrix()[a][b] << " ";
                        }
                        for (int b = 0; b < rows; b++) {
                            std::cout << std::fixed << std::setprecision(2) << B.getMatrix()[a][b] << " ";
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }

        std::cout << "Diagonal normalization:" << std::endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                B.setElem(i, j, B.getMatrix()[i][j] / A.getMatrix()[i][i]);
            }
            A.setElem(i, i, 1);
        }

        for (int a = 0; a < rows; a++) {
            for (int b = 0; b < rows; b++) {
                std::cout << std::fixed << std::setprecision(2) << A.getMatrix()[a][b] << " ";
            }
            for (int b = 0; b < rows; b++) {
                std::cout << std::fixed << std::setprecision(2) << B.getMatrix()[a][b] << " ";
            }
            std::cout << std::endl;
        }
        return B;
    }

    void SLAE(ColumnVector B) const {
        SquareMatrix A = *this;
        std::cout << "Gaussian process:" << std::endl;
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

                std::cout << "step #" << step++ << ": permutation" << std::endl;
                std::cout << A;
                std::cout << B;
            }

            // eliminations
            for (int j = i + 1; j < rows; j++) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    std::cout << "step #" << step++ << ": elimination" << std::endl;
                    std::cout << A;
                    std::cout << B;
                }
            }
        }

        for (int i = rows - 1; i > 0; i--) {
            for (int j = i - 1; j >= 0; j--) {
                if (A.getMatrix()[j][i] != 0) {
                    EliminationMatrix E(j + 1, i + 1, A);
                    A = E * A;
                    B = E * B;

                    std::cout << "step #" << step++ << ": elimination" << std::endl;
                    std::cout << A;
                    std::cout << B;
                }
            }
        }

        std::cout << "Diagonal normalization:" << std::endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                B.setElem(i, j, B.getMatrix()[i][j] / A.getMatrix()[i][i]);
            }
            A.setElem(i, i, 1);
        }

        std::cout << A;
        std::cout << B;
        std::cout << "Result:" << std::endl << B;
        exit(0);
        return;
    }
};

int main() {

    int sA;
    std::cin >> sA;
    SquareMatrix A(sA);
    std::cin >> A;

    int nCV;
    std::cin >> nCV;
    ColumnVector b(nCV);
    std::cin >> b;

    A.SLAE(b);
}