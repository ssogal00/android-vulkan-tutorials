//
// Created by haminlove on 2024-12-27.
//

#include "Matrix.h"

Matrix::Matrix(float32x4_t row0, float32x4_t row1, float32x4_t row2, float32x4_t row3)
{
    mRows[0]=row0;
    mRows[1]=row1;
    mRows[2]=row2;
    mRows[3]=row3;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    return Matrix{};
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix rhsTranspose = rhs.Transpose();
    return Matrix{};
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

void Matrix::DebugPrint() const
{
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
}
