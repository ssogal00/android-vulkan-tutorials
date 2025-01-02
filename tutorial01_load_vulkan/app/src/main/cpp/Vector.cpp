
#include "Vector.h"

#include <android/log.h>
#include <cmath>

float Vector4::X()
{
    float32x2_t low = vget_low_f32(mValue);
    return vget_lane_f32(low,0);
}

float Vector4::Y()
{
    float32x2_t low = vget_low_f32(mValue);
    return vget_lane_f32(low,1);
}

float Vector4::Z()
{
    float32x2_t high = vget_high_f32(mValue);
    return vget_lane_f32(high,0);
}

float Vector4::W()
{
    float32x2_t high = vget_high_f32(mValue);
    return vget_lane_f32(high,0);
}

Vector4::Vector4(float fX, float fY, float fZ, float fW)
{
    float values[4] = {fX, fY, fZ, fW};
    mValue = vld1q_f32(values);
}

Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
{
    Vector4 result;
    result.mValue = vaddq_f32(lhs.mValue, rhs.mValue);
    return result;
}

Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
{
    Vector4 result{ vsubq_f32(lhs.mValue, rhs.mValue)};
    return result;
}

float Dot(const Vector4& lhs, const Vector4& rhs)
{
    float32x4_t sum = vmulq_f32(lhs.mValue, rhs.mValue); // elementwise multiply
    float32x2_t intermediate = vadd_f32(vget_high_f32(sum), vget_low_f32(sum));
    return vget_lane_f32(vpadd_f32(intermediate, intermediate), 0);
}

float Vector4::Length() const
{
    float32x4_t sum = vmulq_f32(mValue, mValue); // elementwise multiply
    float32x2_t intermediate = vadd_f32(vget_high_f32(sum), vget_low_f32(sum));
    float squaredLength = vget_lane_f32(vpadd_f32(intermediate, intermediate), 0);
    return std::sqrt(squaredLength);
}

float Vector4::SquareLength() const
{
    float32x4_t sum = vmulq_f32(mValue, mValue); // elementwise multiply
    float32x2_t intermediate = vadd_f32(vget_high_f32(sum), vget_low_f32(sum));
    float squaredLength = vget_lane_f32(vpadd_f32(intermediate, intermediate), 0);
    return squaredLength;
}


void Vector4::Normalize()
{
    float fInvLength = 1.f / Length();
    float32x4_t intermediate = vdupq_n_f32(fInvLength);
    mValue = vmulq_f32(mValue, intermediate);
}

Vector4 Vector4::Normalize() const
{
    float fInvLength = 1.f / Length();
    float32x4_t intermediate = vdupq_n_f32(fInvLength);
    Vector4 result{};
    result.mValue = vmulq_f32(mValue, intermediate);
    return result;
}

void Vector4::DebugPrintValue()
{
    float value0 = vgetq_lane_f32(mValue, 0);
    float value1 = vgetq_lane_f32(mValue, 1); //
    float value2 = vgetq_lane_f32(mValue, 2); //
    float value3 = vgetq_lane_f32(mValue, 3); //

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "[%.4f,%.4f,%.4f,%.4f]", value0, value1, value2, value3);
}

void Vector4::Vector4Test()
{
    Vector4 A{1,2,3,4};
    Vector4 B{4,5,6,7};
    Vector4 C = A+B;

    A.DebugPrintValue();
    B.DebugPrintValue();
    C.DebugPrintValue();

    float DotAB = Dot(A,B);

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "[%.4f]", DotAB);

    A.Normalize();
    A.DebugPrintValue();
    B.Normalize();
    B.DebugPrintValue();
}
