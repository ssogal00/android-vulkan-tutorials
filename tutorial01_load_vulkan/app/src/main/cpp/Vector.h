

#ifndef ASSEMBLYTEST_VECTOR_H
#define ASSEMBLYTEST_VECTOR_H

#include <arm_neon.h>

class alignas(16) Vector2
{
public:
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);

    float X();
    float Y();
protected:
    float32x2_t mValue;
};

class alignas(16) Vector3
{
public:
    Vector3(){}

    Vector3(float fX, float fY, float fZ);
    Vector3(float32x4_t InValue) : mValue(InValue){}

protected:
    float32x4_t mValue;

};

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

    float X() ;
    float Y() ;
    float Z() ;
    float W() ;

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
