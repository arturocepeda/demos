
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

struct GEColor
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;

    GEColor()
    {
        set(0xFF, 0xFF, 0xFF);
    }

    GEColor(unsigned char cR, unsigned char cG, unsigned char cB, unsigned char cA = 0xFF)
    {
        set(cR, cG, cB, cA);
    }

    void set(unsigned char cR, unsigned char cG, unsigned char cB, unsigned char cA = 0xFF)
    {
        R = cR;
        G = cG;
        B = cB;
        A = cA;
    }
};

struct GEVector3
{
    float X;
    float Y;
    float Z;

    GEVector3()
    {
        set(0.0f, 0.0f, 0.0f);
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
