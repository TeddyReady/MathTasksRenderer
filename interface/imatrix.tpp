#ifndef MATRIXINTERFACE_TPP
#define MATRIXINTERFACE_TPP
#include "interface.h"
#include "matrix.tpp"
#include "residue.h"
#include "complex.tpp"

enum class MatrixOptions
{
    Sum, Diff, Multy,
      Inverse, Det
};

template <class T>
class MatrixInterface : public TaskInterface {
private:
    std::pair<int, int> rangeSize, rangeK;
    MatrixOptions option;
    QRandomGenerator *gen;
    Matrix<T> matrix_1, matrix_2;
    size_t rows, cols;
    int module;

public:
    explicit MatrixInterface(std::pair<int, int> rangeSize, MatrixOptions option)
        : rangeSize(rangeSize), option(option), gen(QRandomGenerator::global()) {}
    ~MatrixInterface() { gen = nullptr; }

    void create()
    {
        rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
        cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
        module = 0;

        switch (option)
        {
        case MatrixOptions::Sum:
        case MatrixOptions::Diff:
            createMatrix(matrix_1, rows, cols);
            createMatrix(matrix_2, rows, cols);
            break;
        case MatrixOptions::Multy:

            createMatrix(matrix_1, rows, cols);
            createMatrix(matrix_2, cols, rows);
            break;

        default:
            createMatrix(matrix_1, rows, rows);
            break;
        }
    }

    QString description()
    {
        switch (option)
        {
        case MatrixOptions::Sum:
            return QString("Вычислите сумму матриц");
        case MatrixOptions::Diff:
            return QString("Найдите разность матриц");
        case MatrixOptions::Multy:
            return QString("Найдите произведение матриц");
        case MatrixOptions::Inverse:
            return QString("Найдите матрицу, обратную данной");
        case MatrixOptions::Det:
            return QString("Вычислить детерминант матрицы");
        default:
            break;
        }

        return QString("");
    }

    QString task()
    {
        switch (option)
        {
        case MatrixOptions::Sum:
            return getType() + printMatrix(matrix_1) + "+" + printMatrix(matrix_2) + "=~?";
        case MatrixOptions::Diff:
            return getType() + printMatrix(matrix_1) + "-" + printMatrix(matrix_2) + "=~?";
        case MatrixOptions::Multy:
            return getType() + printMatrix(matrix_1) + "\\cdot " + printMatrix(matrix_2) + "=~?";
        case MatrixOptions::Inverse:
            return getType() + "{" + printMatrix(matrix_1) + "}^{-1}=~?";
        case MatrixOptions::Det:
            return getType() + printMatrix(matrix_1).replace("pmatrix", "vmatrix") + "=~?";
        default:
            break;
        }

        return QString("");
    }

    QString answer()
    {
        switch (option)
        {
        case MatrixOptions::Sum:
            return printMatrix(matrix_1 + matrix_2);
        case MatrixOptions::Diff:
            return printMatrix(matrix_1 - matrix_2);
        case MatrixOptions::Multy:
            return printMatrix(matrix_1 * matrix_2);
        case MatrixOptions::Inverse:
            return QString("\\frac{1}{%1}%2").arg(stringFromT(matrix_1.det())).arg(printMatrix(Matrix<T>(~matrix_1)));
        case MatrixOptions::Det:
            return stringFromT(matrix_1.det());
        default:
            break;
        }

        return QString("");
    }

    QString printMatrix(const Matrix<T> &matrix)
    {
        QString result = "\\begin{pmatrix}";
        for (std::size_t i = 0; i < matrix.get().size(); ++i) {
            for (std::size_t j = 0; j < matrix.get().at(i).size(); ++j) {
                result += stringFromT(matrix.get().at(i).at(j));
                if (j + 1 != matrix.get().at(i).size()) result += "&";
                else result += "\\\\";
            }
        } result += "\\end{pmatrix}";
        return result;
    }

    void createMatrix(Matrix<int> &matrix, size_t rows, size_t cols)
    {
        matrix = Matrix<int>(rows, cols);

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, static_cast<int>(gen->bounded(-20, 20)));
    }

    void createMatrix(Matrix<double> &matrix, size_t rows, size_t cols)
    {
        matrix = Matrix<double>(rows, cols);

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, static_cast<double>(gen->bounded(-20, 20)));
    }

    void createMatrix(Matrix<Zn> &matrix, size_t rows, size_t cols)
    {
        matrix = Matrix<Zn>(rows, cols);

        if (module == 0)
            module = static_cast<int>(gen->bounded(2, 62));

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, Zn(static_cast<int>(gen->bounded(0, module)), module));
    }

    void createMatrix(Matrix<Zp> &matrix, size_t rows, size_t cols)
    {
        matrix = Matrix<Zp>(rows, cols);

        if (module == 0)
        {
            do {
                module = static_cast<int>(gen->bounded(2, 62));
            } while(!isPrime(module));
        }

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, Zp(static_cast<int>(gen->bounded(0, module)), module));
    }

    void createMatrix(Matrix<Complex<int>> &matrix, size_t rows, size_t cols)
    {
        matrix = Matrix<Complex<int>>(rows, cols);

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, Complex<int>(static_cast<int>(gen->bounded(-20, 20)), static_cast<int>(gen->bounded(-20, 20))));
    }

    void createMatrix(Matrix<Complex<double>> &matrix, size_t rows, size_t cols)
    {
        matrix = Matrix<Complex<double>>(rows, cols);

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, Complex<double>(static_cast<double>(gen->bounded(-20, 20)), static_cast<double>(gen->bounded(-20, 20))));
    }

    QString stringFromT(const T &param) const;
    QString getType();
};

#endif // MATRIXINTERFACE_TPP
