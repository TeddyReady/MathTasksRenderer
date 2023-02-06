#ifndef MATRIX_TPP
#define MATRIX_TPP

#include <QRandomGenerator>
#include <QString>

#include <vector>
#include <cmath>

enum class MatrixOptions {
    Sum, Diff, Multy,
        Inverse
};

template <class T>
class Matrix {
public:
    Matrix(): rows(0), cols(0), gen(QRandomGenerator::global()) {}
    Matrix(std::size_t rows, std::size_t cols, T diagonalValue): rows(rows), cols(cols),
        gen(QRandomGenerator::global())
    {
        data.resize(rows);
        for(std::size_t i = 0; i < rows; ++i){
            for(std::size_t j = 0; j < cols; ++j){
                if (i == j)
                    data[i].emplace_back(diagonalValue);
                else data[i].emplace_back(0);
            }
        }
    }

    Matrix<T> operator +(const Matrix<T> &matrix) const
    {
        if (rows == matrix.rows && cols == matrix.cols){
            Matrix<T> tmp(rows, cols, 0);

            for (std::size_t i = 0; i < rows; ++i)
                for (std::size_t j = 0; j < cols; ++j)
                    tmp.data[i][j] = data[i][j] + matrix.data[i][j];

            return tmp;
        }
        return *this;
    }
    Matrix<T> operator -(const Matrix<T> &matrix) const
    {
        if(rows == matrix.rows && cols == matrix.cols) {
            Matrix<T> tmp(rows, cols, 0);

            for (std::size_t i = 0; i < rows; ++i)
                for (std::size_t j = 0; j < cols; ++j)
                    tmp.data[i][j] = data[i][j] - matrix.data[i][j];

            return tmp;
        }
        return *this;
    }
    Matrix<T> operator *(const Matrix<T> &matrix) const
    {
        if (matrix.rows == cols && rows == matrix.cols) {
            Matrix<T> tmp(rows, matrix.cols, 0);

            for(std::size_t i = 0; i < rows; ++i)
                for(std::size_t j = 0; j < matrix.cols; ++j)
                    for(std::size_t k = 0; k < matrix.rows; ++k)
                        tmp.data[i][j] += data[i][k] * matrix.data[k][j];

            return tmp;
        }
        return *this;
    }
    Matrix<T> operator ~() const
    {
        if(isSquare() && det() != 0) {
            Matrix<T> tmp(rows, cols, 0);
            for (std::size_t i = 0; i < rows; ++i)
            {
                for (std::size_t j = 0; j < cols; ++j)
                    tmp.data[i][j] = minor(i, j).det() * ((1 / det()) * pow(-1, i + j));

            }
            return tmp.trans();
        }
        return *this;
    }

    bool isSquare() const { return (cols != rows) ? false : true; }
    double det() const
    {
        if (isSquare()) {
            if (rows == 1)
                return data[0][0];
            else if (rows == 2)
                return (data[0][0] * data[1][1] - data[0][1] * data[1][0]);
            else {
                Matrix<T> tmp(rows - 1, rows - 1, 0);
                double det = 0;
                std::size_t k1, k2;
                for(size_t i = 0; i < rows; ++i) {
                    k1 = 0;
                    for(size_t j = 1; j < rows; ++j) {
                        k2 = 0;
                        for(size_t k = 0; k < rows; k++) {
                            if (k != i) {
                                tmp.data[k1][k2] = data[j][k];
                                ++k2;
                            }
                        }
                        ++k1;
                    }
                    det += pow(-1, i + 2) * data[0][i] * tmp.det();
                }
                return det;
            }
        }
        return -100000000;
    }
    Matrix<T> trans() const
    {
        Matrix<T> matrix(cols, rows, 0);

        for(int i = 0; i < cols; ++i)
            for(int j = 0; j < rows; ++j)
                matrix.data[i][j] = data[j][i];

        return matrix;
    }
    Matrix<T> minor(std::size_t x, std::size_t y) const
    {
        if(isSquare()){
            Matrix<T> tmp(rows - 1, rows - 1, 0);
            size_t k1 = 0;
            for (std::size_t i = 0; i < rows; ++i)
            {
                int k2 = 0;
                for (std::size_t j = 0; j < rows; ++j)
                {
                    if ((i != x) && (j != y)) {
                        tmp.data[k1][j] = data[i][j];
                        ++k2;
                        if (k2 % (rows - 1) == 0) ++k1;
                    }
                }
            }
            return tmp;
        }
        return *this;
    }
    QString getMatrix() const
    {
        QString result = "\\left(\\begin{smallmatrix}";
        for (std::size_t i = 0; i < data.size(); ++i) {
            for (std::size_t j = 0; j < data[i].size(); ++j) {
                result += QString::number(data[i][j]);
                if (j + 1 != data[i].size()) result += "&";
                else result += "\\\\";
            }
        } result += "\\end{smallmatrix}\\right)";
        return result;
    }
    void setTask(std::size_t rows, std::size_t cols, int minN, int maxN)
    {
        data.clear();
        this->rows = rows;
        this->cols = cols;
        data.resize(rows);
        for (std::size_t i = 0; i < rows; ++i)
            for(std::size_t j = 0; j < cols; ++j)
                data[i].emplace_back(gen->bounded(minN, maxN + 1));
    }
private:
    std::size_t rows, cols;
    std::vector<std::vector<T>> data;
    QRandomGenerator *gen;
};

#endif // MATRIX_TPP
