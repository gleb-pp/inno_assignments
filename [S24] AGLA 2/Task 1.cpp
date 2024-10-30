#include <iostream>
#include <vector>

class Matrix {
private:
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

    void operator=(const Matrix& other) {
        rows = other.rows;
        columns = other.columns;
        matrix = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    Matrix operator+(const Matrix& other) const {
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

    Matrix operator-(const Matrix& other) const {
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

    Matrix operator*(const Matrix& other) const {
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

    Matrix transpose() const {
        Matrix result(columns, rows);
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                result.matrix[j][i] = matrix[i][j];
            }
        }
        return result;
    }

};

int main() {
    
    int rA, cA;
    std::cin >> rA >> cA;
    Matrix A(rA, cA);
    std::cin >> A;

    int rB, cB;
    std::cin >> rB >> cB;
    Matrix B(rB, cB);
    std::cin >> B;

    int rC, cC;
    std::cin >> rC >> cC;
    Matrix C(rC, cC);
    std::cin >> C;

    Matrix D = A + B;
    Matrix E = B - A;
    Matrix F = C * A;
    Matrix G = A.transpose();
    
    std::cout << D;
    std::cout << E;
    std::cout << F;
    std::cout << G;
}