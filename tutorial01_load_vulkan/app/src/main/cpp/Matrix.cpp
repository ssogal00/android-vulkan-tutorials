//
// Created by haminlove on 2024-12-27.
//

#include "Matrix.h"
#include <cmath>

const Matrix Matrix::Identity
{
    float32x4_t {1,0,0,0},
    float32x4_t {0,1,0,0},
    float32x4_t {0,0,1,0},
    float32x4_t {0,0,0,1},
};

Matrix Matrix::Ortho(float l, float r, float b , float t, float n , float f)
{
    Matrix Result{};
    float32x4_t rows0{ 2.f/(r-l), 0, 0, 0};
    float32x4_t rows1{ 0, 2.f/(t-b), 0, 0};
    float32x4_t rows2{ 0, 0, -2.f/(f-n), 0};
    float32x4_t rows3{ -1* (r+l)/(r-l), -1 * (t+b)/(t-b), -1.f * (f+n)/(f-n), 1.f };

    Result.mRows[0] = rows0;
    Result.mRows[1] = rows1;
    Result.mRows[2] = rows2;
    Result.mRows[3] = rows3;

    return Result;
}



Matrix Matrix::Perspective(float fov, float aspect, float near, float far)
{
    Matrix Result{};

    float const a = 1.f / tanf(fov * 0.5f);

    float32x4_t rows0{ a/aspect, 0, 0, 0};
    float32x4_t rows1{ 0, a, 0, 0};
    float m22 = -((far+near)/(far-near));
    float m32 = -((2.f * far * near)/(far-near));
    float32x4_t rows2{ 0,0, m22 ,-1};
    float32x4_t rows3{ 0,0, m32 ,0};

    Result.mRows[0] = rows0;
    Result.mRows[1] = rows1;
    Result.mRows[2] = rows2;
    Result.mRows[3] = rows3;

    return Result;
}



Matrix::Matrix(float32x4_t row0, float32x4_t row1, float32x4_t row2, float32x4_t row3)
{
    mRows[0]=row0;
    mRows[1]=row1;
    mRows[2]=row2;
    mRows[3]=row3;
}

Matrix::Matrix()
{
    mRows[0]=float32x4_t{0,0,0,0};
    mRows[1]=float32x4_t{0,0,0,0};
    mRows[2]=float32x4_t{0,0,0,0};
    mRows[3]=float32x4_t{0,0,0,0};
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    Matrix result{};
    for (int i = 0; i < 4; i++)
    {
        float32x4_t sum = vmovq_n_f32(0.0f);
        {
            float32x4_t a = vdupq_n_f32(vgetq_lane_f32(lhs.mRows[i], 0));
            sum = vmlaq_f32(sum, a, rhs.mRows[0]);

            a = vdupq_n_f32(vgetq_lane_f32(lhs.mRows[i], 1));
            sum = vmlaq_f32(sum, a, rhs.mRows[1]);

            a = vdupq_n_f32(vgetq_lane_f32(lhs.mRows[i], 2));
            sum = vmlaq_f32(sum, a, rhs.mRows[2]);

            a = vdupq_n_f32(vgetq_lane_f32(lhs.mRows[i], 3));
            sum = vmlaq_f32(sum, a, rhs.mRows[3]);
        }
        result.mRows[i] = sum;
    }
    return result;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix result{};
    result.mRows[0] = vaddq_f32(lhs.mRows[0], rhs.mRows[0]);
    result.mRows[1] = vaddq_f32(lhs.mRows[1], rhs.mRows[1]);
    result.mRows[2] = vaddq_f32(lhs.mRows[2], rhs.mRows[2]);
    result.mRows[3] = vaddq_f32(lhs.mRows[3], rhs.mRows[3]);

    return result;
}

Matrix RotateZ(const Matrix& lhs, const float angle)
{
    Matrix Result{};
    float s = std::sinf(angle);
    float c = std::cosf(angle);

    Matrix R
    {
            float32x4_t{c,s,0,0},
            float32x4_t{-s,c,0,0},
            float32x4_t{0,0,1,0},
            float32x4_t {0,0,0,1}
    };

    return R * lhs;
}

Matrix RotateX(const Matrix& lhs, const float angle)
{
    Matrix Result{};
    float s = std::sinf(angle);
    float c = std::cosf(angle);

    Matrix R
    {
            float32x4_t{1,0,0,0},
            float32x4_t{0,c,s,0},
            float32x4_t{0,-s,c,0},
            float32x4_t {0,0,0,1}
    };

    return R * lhs;
}


void Matrix::Transpose()
{
    float32x4x2_t tmp1 = vtrnq_f32(mRows[0], mRows[1]);
    float32x4x2_t tmp2 = vtrnq_f32(mRows[2], mRows[3]);

    mRows[0] = vcombine_f32(vget_low_f32(tmp1.val[0]), vget_low_f32(tmp2.val[0]));
    mRows[1] = vcombine_f32(vget_low_f32(tmp1.val[1]), vget_low_f32(tmp2.val[1]));
    mRows[2] = vcombine_f32(vget_high_f32(tmp1.val[0]), vget_high_f32(tmp2.val[0]));
    mRows[3] = vcombine_f32(vget_high_f32(tmp1.val[1]), vget_high_f32(tmp2.val[1]));
}

Matrix Matrix::Transpose() const
{
    float32x4x2_t tmp1 = vtrnq_f32(mRows[0], mRows[1]);
    float32x4x2_t tmp2 = vtrnq_f32(mRows[2], mRows[3]);
    Matrix Result;
    Result.mRows[0] = vcombine_f32(vget_low_f32(tmp1.val[0]), vget_low_f32(tmp2.val[0]));
    Result.mRows[1] = vcombine_f32(vget_low_f32(tmp1.val[1]), vget_low_f32(tmp2.val[1]));
    Result.mRows[2] = vcombine_f32(vget_high_f32(tmp1.val[0]), vget_high_f32(tmp2.val[0]));
    Result.mRows[3] = vcombine_f32(vget_high_f32(tmp1.val[1]), vget_high_f32(tmp2.val[1]));
    return Result;
}
void Matrix::MatrixTest()
{
    float32x4_t row0{1,2,3,4};
    float32x4_t row1{5,6,7,8};
    float32x4_t row2{9,10,11,12};
    float32x4_t row3{13,14,15,16};

    Matrix temp
    {
        row0, row1, row2, row3
    };

    temp.DebugPrint();
    temp.Transpose();
    temp.DebugPrint();
    temp.Transpose();

    Matrix multTest = temp * temp;
    multTest.DebugPrint();
}

void Matrix::DebugPrint() const
{
    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "===========Matrix4X4===========");

    float value0 = vgetq_lane_f32(mRows[0], 0);
    float value1 = vgetq_lane_f32(mRows[0], 1); //
    float value2 = vgetq_lane_f32(mRows[0], 2); //
    float value3 = vgetq_lane_f32(mRows[0], 3); //

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "|%.4f,%.4f,%.4f,%.4f|", value0, value1, value2, value3);

    value0 = vgetq_lane_f32(mRows[1], 0);
    value1 = vgetq_lane_f32(mRows[1], 1); //
    value2 = vgetq_lane_f32(mRows[1], 2); //
    value3 = vgetq_lane_f32(mRows[1], 3); //

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "|%.4f,%.4f,%.4f,%.4f|", value0, value1, value2, value3);


    value0 = vgetq_lane_f32(mRows[2], 0);
    value1 = vgetq_lane_f32(mRows[2], 1); //
    value2 = vgetq_lane_f32(mRows[2], 2); //
    value3 = vgetq_lane_f32(mRows[2], 3); //

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "|%.4f,%.4f,%.4f,%.4f|", value0, value1, value2, value3);

    value0 = vgetq_lane_f32(mRows[3], 0);
    value1 = vgetq_lane_f32(mRows[3], 1); //
    value2 = vgetq_lane_f32(mRows[3], 2); //
    value3 = vgetq_lane_f32(mRows[3], 3); //

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "|%.4f,%.4f,%.4f,%.4f|", value0, value1, value2, value3);
    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "=============================");
}
