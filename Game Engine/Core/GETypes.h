
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Types
//
//  --- GETypes.h ---
//
//////////////////////////////////////////////////////////////////

#include <cmath>

typedef unsigned char byte;

struct GEColor
{
    float R;
    float G;
    float B;
    float A;

    GEColor()
        : R(1.0f)
        , G(1.0f)
        , B(1.0f)
        , A(1.0f)
    {
    }

    GEColor(byte cR, byte cG, byte cB, byte cA = 0xFF)
    {
        set(cR, cG, cB, cA);
    }

    GEColor(float fR, float fG, float fB, float fA = 1.0f)
    {
        set(fR, fG, fB, fA);
    }

    void set(byte cR, byte cG, byte cB, byte cA = 0xFF)
    {
        R = cR / 255.0f;
        G = cG / 255.0f;
        B = cB / 255.0f;
        A = cA / 255.0f;
    }

    void set(float fR, float fG, float fB, float fA = 1.0f)
    {
        R = fR;
        G = fG;
        B = fB;
        A = fA;
    }
};

struct GEVector3
{
    float X;
    float Y;
    float Z;

    GEVector3()
        : X(0.0f)
        , Y(0.0f)
        , Z(0.0f)
    {
    }

    GEVector3(float vX, float vY, float vZ)
    {
        set(vX, vY, vZ);
    }

    void set(float vX, float vY, float vZ)
    {
        X = vX;
        Y = vY;
        Z = vZ;
    }

    float getLength()
    {
        return sqrt(X * X + Y * Y + Z * Z);
    }

    void normalize()
    {
        float fLength = sqrt(X * X + Y * Y + Z * Z);

        X /= fLength;
        Y /= fLength;
        Z /= fLength;
    }

    GEVector3 operator+(const GEVector3& v)
    {
        return GEVector3(X + v.X, Y + v.Y, Z + v.Z);
    }

    GEVector3& operator+=(const GEVector3& v)
    {
        X += v.X; Y += v.Y; Z += v.Z;
        return *this;
    }

    GEVector3 operator-(const GEVector3& v)
    {
        return GEVector3(X - v.X, Y - v.Y, Z - v.Z);
    }

    GEVector3& operator-=(const GEVector3& v)
    {
        X -= v.X; Y -= v.Y; Z -= v.Z;
        return *this;
    }

    GEVector3 operator*(const float fValue)
    {
        return GEVector3(X * fValue, Y * fValue, Z * fValue);
    }

    GEVector3& operator*=(const float fValue)
    {
        X *= fValue; Y *= fValue; Z *= fValue;
        return *this;
    }

    float operator*(const GEVector3& v)
    {
        return (X * v.X + Y * v.Y + Z * v.Z);
    }
};

struct GEQuaternion
{
    float X;
    float Y;
    float Z;
    float W;

    GEQuaternion()
        : X(0.0f)
        , Y(0.0f)
        , Z(0.0f)
        , W(0.0f)
    {
    }
};
