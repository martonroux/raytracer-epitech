/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** MatrixN.hpp
*/

#ifndef MATRIXN_HPP
#define MATRIXN_HPP

#include <type_traits>
#include <utility>
#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <cstdint>
#include <string>

namespace Math {

    template<uint8_t ROW, uint8_t COL>
    class Matrix {
    private:
        std::vector<std::vector<double>> _values;

    public:
        Matrix();
        explicit Matrix(std::vector<std::vector<double>> values) : _values(std::move(values)) {}
        Matrix(const Matrix& other);
        ~Matrix() = default;

        Matrix operator+(const Matrix& other);
        Matrix& operator+=(const Matrix& other);
        Matrix<ROW, ROW> operator*(const Matrix<COL, ROW>& other);
        Matrix& operator*=(const Matrix<COL, ROW>& other);

        double& operator()(int row, int col);
        double operator()(int row, int col) const;
        Matrix& operator=(const Matrix& other);

        [[nodiscard]] int cols() const { return COL; }
        [[nodiscard]] int rows() const { return ROW; }
        [[nodiscard]] Matrix<COL, ROW> T() const;

        [[nodiscard]] static Matrix<ROW, COL> identity();
        [[nodiscard]] Matrix<ROW, COL> inverse();
    };

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL>::Matrix()
    {
        for (int i = 0; i < ROW; i++) {
            _values.emplace_back();
            for (int j = 0; j < COL; j++) {
                _values[i].push_back(0);
            }
        }
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL>::Matrix(const Matrix &other)
    {
        for (int i = 0; i < ROW; i++) {
            _values.emplace_back();
            for (int j = 0; j < COL; j++) {
                _values[i].push_back(0);
            }
        }
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL> Matrix<ROW, COL>::operator+(const Matrix& other)
    {
        Matrix result;
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL> & Matrix<ROW, COL>::operator+=(const Matrix &other)
    {
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                (*this)(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return *this;
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, ROW> Matrix<ROW, COL>::operator*(
        const Matrix<COL, ROW> &other)
    {
        Matrix<ROW, ROW> matRes;

        for (int leftRow = 0; leftRow < ROW; leftRow++) {
            for (int rightCol = 0; rightCol < ROW; rightCol++) {
                double res = 0;

                for (int i = 0; i < COL; i++) {
                    res += (*this)(leftRow, i) * other(i, rightCol);
                }
                matRes(leftRow, rightCol) = res;
            }
        }
        return matRes;
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL> & Matrix<ROW, COL>::operator*=(
        const Matrix<COL, ROW> &other)
    {
        for (int leftRow = 0; leftRow < ROW; leftRow++) {
            for (int rightCol = 0; rightCol < ROW; rightCol++) {
                double res = 0;

                for (int i = 0; i < COL; i++) {
                    res += (*this)(leftRow, i) * other(i, rightCol);
                }
                (*this)(leftRow, rightCol) = res;
            }
        }
        return *this;
    }

    template<uint8_t ROW, uint8_t COL, uint8_t INNER>
    Matrix<ROW, COL> operator*(const Matrix<ROW, INNER>& left, const Matrix<INNER, COL>& right)
    {
        Matrix<ROW, COL> matRes;

        for (int leftRow = 0; leftRow < ROW; leftRow++) {
            for (int rightCol = 0; rightCol < COL; rightCol++) {
                double res = 0;

                for (int i = 0; i < INNER; i++) {
                    res += left(leftRow, i) * right(i, rightCol);
                }
                matRes(leftRow, rightCol) = res;
            }
        }
        return matRes;
    }

    template<uint8_t ROW, uint8_t COL>
    double& Matrix<ROW, COL>::operator()(int row, int col)
    {
        if (col >= COL || row >= ROW) {
            throw std::exception();
        }
        return _values[row][col];
    }

    template<uint8_t ROW, uint8_t COL>
    double Matrix<ROW, COL>::operator()(int row, int col) const
    {
        if (row >= ROW || col >= COL) {
            throw std::exception();
        }
        return _values[row][col];
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL> & Matrix<ROW, COL>::operator=(const Matrix &other)
    {
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                (*this)(i, j) = other(i, j);
            }
        }
        return *this;
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<COL, ROW> Matrix<ROW, COL>::T() const
    {
        Matrix<COL, ROW> matRes;

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                matRes(j, i) = (*this)(i, j);
            }
        }
        return matRes;
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL> Matrix<ROW, COL>::identity()
    {
        Matrix matRes;

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                if (i == j) {
                    matRes(i, j) = 1;
                } else {
                    matRes(i, j) = 0;
                }
            }
        }
        return matRes;
    }

    template<uint8_t ROW, uint8_t COL>
    Matrix<ROW, COL> Matrix<ROW, COL>::inverse()
    {
        if (ROW != COL) {
            throw std::runtime_error("Inverse only defined for square matrices.");
        }
        Matrix<ROW, COL*2> augmented;
        for (int i = 0; i < ROW; ++i) {
            for (int j = 0; j < COL; ++j) {
                augmented(i, j) = (*this)(i, j);
            }
            for (int j = COL; j < 2 * COL; ++j) {
                augmented(i, j) = (i == (j - COL)) ? 1.0 : 0.0;
            }
        }

        for (int col = 0; col < COL; ++col) {
            if (augmented(col, col) == 0) {
                int swap_row = col + 1;
                while (swap_row < ROW && augmented(swap_row, col) == 0) {
                    ++swap_row;
                }
                if (swap_row == ROW) {
                    throw std::runtime_error("Matrix is singular and cannot be inverted.");
                }
                for (int j = 0; j < 2 * COL; ++j) {
                    std::swap(augmented(col, j), augmented(swap_row, j));
                }
            }
            double pivot = augmented(col, col);
            for (int j = 0; j < 2 * COL; ++j) {
                augmented(col, j) /= pivot;
            }
            for (int row = 0; row < ROW; ++row) {
                if (row != col) {
                    double factor = augmented(row, col);
                    for (int j = 0; j < 2 * COL; ++j) {
                        augmented(row, j) -= factor * augmented(col, j);
                    }
                }
            }
        }
        Matrix<ROW, COL> inverse;
        for (int i = 0; i < ROW; ++i) {
            for (int j = 0; j < COL; ++j) {
                inverse(i, j) = augmented(i, j + COL);
            }
        }

        return inverse;
    }


    template<uint8_t ROW, uint8_t COL>
    std::ostream& operator<<(std::ostream& os, const Matrix<ROW, COL>& matrix){
        for (int i = 0; i < matrix.rows(); i++) {
            os << "[ ";
            for (int j = 0; j < matrix.cols(); j++) {
                os << std::to_string(matrix(i, j));
                if (j < matrix.cols() - 1)
                    os << "\t";
            }
            os << " ]" << std::endl;
        }
        return os;
    }
}



#endif
