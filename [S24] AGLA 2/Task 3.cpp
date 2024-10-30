#include <iostream>
#include <vector>

class Matrix {
protected:
    std::vector<std::vector<int>> matrix;
    int rows;
    int columns;

public:
    Matrix(int r, int c) {
        rows = r;
        columns = c;
        matrix = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
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
                out << mat.matrix[i][j] << " ";
            }
            out << std::endl;
        }
        return out;
    }

    virtual void operator=(const Matrix& other) {
        rows = other.rows;
        columns = other.columns;
        matrix = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
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
};

class IdentityMatrix : public Matrix {
public:
    IdentityMatrix(int size) : Matrix(size, size) {
        for (int i = 0; i < size; ++i) {
            matrix[i][i] = 1;
        }
    }
};

class EliminationMatrix : public Matrix {
public:
    EliminationMatrix(int i, int j, Matrix A) : Matrix(A.getRows(), A.getColumns()) {
        if (i > 0 && i <= A.getRows() && j > 0 && j <= A.getColumns()) {
            if (A.getMatrix()[i - 1][j - 1] != 0) {
                int k = 0;
                bool ready = false;
                while (k < (A.getRows() - 1)) {
                    int elem = A.getMatrix()[(i + k) % A.getRows()][j - 1];
                    if (elem != 0) {
                        for (int a = 0; a < rows; a++) {
                            for (int b = 0; b < columns; b++) {
                                if (a == (i-1) && b == ((i + k) % A.getRows())) {
                                    matrix[a][b] = - (A.getMatrix()[i-1][j-1] / elem);
                                    if (matrix[a][b] != 0) {
                                        ready == true;
                                    }
                                } else if (a == b) {
                                    matrix[a][b] = 1;
                                } else {
                                    matrix[a][b] = 0;
                                }
                            }
                        }
                        if (ready) {
                            break;
                        }
                    }
                    k++;
                }
            } else {
                for (int a = 0; a < rows; a++) {
                    for (int b = 0; b < columns; b++) {
                        matrix[a][b] = (int)(a == b);
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

int main() {

    int sA;
    std::cin >> sA;
    SquareMatrix A(sA);
    std::cin >> A;

    IdentityMatrix I(3);
    std::cout << I;

    EliminationMatrix E(2, 1, A);
    std::cout << E;

    Matrix B = E * A;
    std::cout << B;

    PermutationMatrix P(2, 1, A);
    std::cout << P;

    Matrix C = P * A;
    std::cout << C;

}