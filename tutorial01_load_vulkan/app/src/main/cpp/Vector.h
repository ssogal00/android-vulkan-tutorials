

#ifndef ASSEMBLYTEST_VECTOR_H
#define ASSEMBLYTEST_VECTOR_H

#include <arm_neon.h>


class alignas(16) Vector4
{
public:
    Vector4(){}
    Vector4(float fX, float fY, float fZ, float fW);
    Vector4(float32x4_t InValue) : mValue(InValue){}

    friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
    friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
    friend float Dot(const Vector4& lhs, const Vector4& rhs);
    friend Vector4 Cross(const Vector4& lhs, const Vector4& rhs);

    void DebugPrintValue();

    void Normalize();
    Vector4 Normalize() const;
    float Length() const;
    float SquareLength() const;

    static void Vector4Test();
protected:
    float32x4_t mValue;
};

Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
float Dot(const Vector4& lhs, const Vector4& rhs);


#endif //ASSEMBLYTEST_VECTOR_H
