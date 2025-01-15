
#ifndef ASSEMBLYTEST_MATRIX_H
#define ASSEMBLYTEST_MATRIX_H

#include "Vector.h"
#include <arm_neon.h>
#include <android/log.h>

#define TO_RADIAN(degree) ((degree) / (3.141592653f / 180.f))

class alignas(16) Matrix
{
public:
    Matrix();
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

    static void MatrixTest();

    static Matrix Ortho(float l, float r, float b , float t, float n , float f);

    static Matrix Perspective(float fov, float aspect, float near, float far);

    const static Matrix Identity;
    const static Matrix Zero;
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs);

Matrix RotateZ(const Matrix& lhs, const float angle);

Matrix RotateX(const Matrix& lhs, const float angle);

Matrix RotateY(const Matrix& lhs, const float angle);

#endif //ASSEMBLYTEST_MATRIX_H
