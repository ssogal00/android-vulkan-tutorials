
#ifndef ASSEMBLYTEST_MATRIX_H
#define ASSEMBLYTEST_MATRIX_H

#include "Vector.h"
#include <arm_neon.h>
#include <android/log.h>


class alignas(16) Matrix
{
public:
    Matrix(){}
    Matrix(float32x4_t row0, float32x4_t row1, float32x4_t row2, float32x4_t row3);
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);

    Vector4 Transform(const Vector4& InVec) const;

    void Transpose();
    Matrix Transpose() const;

    void Inverse();
    Matrix Inverse() const;

    float32x4_t mRows[4];

    void DebugPrint() const;
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs);

#endif //ASSEMBLYTEST_MATRIX_H
