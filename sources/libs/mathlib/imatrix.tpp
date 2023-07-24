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

public:
    explicit MatrixInterface(std::pair<int, int> rangeSize, MatrixOptions option)
        : rangeSize(rangeSize), option(option), gen(QRandomGenerator::global()) {}
    ~MatrixInterface() { gen = nullptr; }

    void create()
    {
        switch (option)
        {
        case MatrixOptions::Sum:
        case MatrixOptions::Diff:
        case MatrixOptions::Multy:

            createMatrix(matrix_1);
            createMatrix(matrix_2);
            break;

        default:
            createMatrix(matrix_1);
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
        }
    }

    QString task()
    {
        switch (option)
        {
        case MatrixOptions::Sum:
            return printMatrix(matrix_1) + "+" + printMatrix(matrix_2) + "=~?";
        case MatrixOptions::Diff:
            return printMatrix(matrix_1) + "-" + printMatrix(matrix_2) + "=~?";
        case MatrixOptions::Multy:
            return printMatrix(matrix_1) + "\\cdot" + printMatrix(matrix_2) + "=~?";
        case MatrixOptions::Inverse:
            return "{" + printMatrix(matrix_1) + "}^{-1}=~?";
        case MatrixOptions::Det:
            return printMatrix(matrix_1).replace("pmatrix", "vmatrix") + "=~?";
        }
    }

    QString answer()
    {
//        switch (option)
//        {
//        case MatrixOptions::Sum:
//            return printMatrix(matrix_1 + matrix_2);
//        case MatrixOptions::Diff:
//            return printMatrix(matrix_1 - matrix_2);
//        case MatrixOptions::Multy:
//            return printMatrix(matrix_1 * matrix_2);
//        case MatrixOptions::Inverse:
//            return QString("\\frac{1}{" + QString::number(matrix_1.det()) + "}") + printMatrix(Matrix<T>(~matrix_1));
//        case MatrixOptions::Det:
//            return QString::number(matrix_1.det());
//        }
    }

    QString printMatrix(const Matrix<T> &matrix)
    {
        QString result = "\\begin{pmatrix}";
        for (std::size_t i = 0; i < matrix.get().size(); ++i) {
            for (std::size_t j = 0; j < matrix.get().at(i).size(); ++j) {
                result += QString::number(matrix.get().at(i).at(j));
                if (j + 1 != matrix.get().at(i).size()) result += "&";
                else result += "\\\\";
            }
        } result += "\\end{pmatrix}";
        return result;
    }

    void createMatrix(Matrix<int> &matrix)
    {
        size_t rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
        size_t cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));

        matrix = Matrix<int>(rows, cols);

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, static_cast<int>(gen->bounded(-20, 20)));
    }

    void createMatrix(Matrix<double> &matrix)
    {
        size_t rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
        size_t cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));

        matrix = Matrix<double>(rows, cols);

        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                matrix.set(i, j, static_cast<double>(gen->bounded(-20, 20)));
    }

//    void createMatrix(Matrix<Zn> &matrix)
//    {
//        size_t rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
//        size_t cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));

//        matrix = Matrix<Zn>(rows, cols);

//        int module = static_cast<int>(gen->bounded(2, 62));
//        for(size_t i = 0; i < rows; ++i)
//            for(size_t j = 0; j < cols; ++j)
//                matrix.set(i, j, Zn(static_cast<int>(gen->bounded(0, module), module)));
//    }

//    void createMatrix(Matrix<Zp> &matrix)
//    {
//        size_t rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
//        size_t cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));

//        matrix = Matrix<Zp>(rows, cols);

//        int module;
//        do {
//            module = static_cast<int>(gen->bounded(2, 62));
//        } while(!isPrime(module));

//        for(size_t i = 0; i < rows; ++i)
//            for(size_t j = 0; j < cols; ++j)
//                matrix.set(i, j, Zp(static_cast<int>(gen->bounded(0, module), module)));
//    }

//    void createMatrix(Matrix<Complex<int>> &matrix)
//    {
//        size_t rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
//        size_t cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));

//        matrix = Matrix<Complex<int>>(rows, cols);

//        for(size_t i = 0; i < rows; ++i)
//            for(size_t j = 0; j < cols; ++j)
//                matrix.set(i, j, Complex<int>(static_cast<int>(gen->bounded(-20, 20)), static_cast<int>(gen->bounded(-20, 20))));
//    }

//    void createMatrix(Matrix<Complex<double>> &matrix)
//    {
//        size_t rows = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));
//        size_t cols = static_cast<size_t>(gen->bounded(rangeSize.first, rangeSize.second));

//        matrix = Matrix<Complex<double>>(rows, cols);

//        for(size_t i = 0; i < rows; ++i)
//            for(size_t j = 0; j < cols; ++j)
//                matrix.set(i, j, Complex<double>(static_cast<double>(gen->bounded(-20, 20)), static_cast<double>(gen->bounded(-20, 20))));
//    }
};

#endif // MATRIXINTERFACE_TPP
