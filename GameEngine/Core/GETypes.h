
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

#pragma once

#include <cmath>
#include <cstring>

typedef unsigned char byte;
typedef unsigned int uint;

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

struct GEVector2
{
    float X;
    float Y;

    GEVector2()
        : X(0.0f)
        , Y(0.0f)
    {
    }

    GEVector2(float vX, float vY)
    {
        set(vX, vY);
    }

    void set(float vX, float vY)
    {
        X = vX;
        Y = vY;
    }

    float getLength()
    {
        return sqrt(X * X + Y * Y);
    }

    void normalize()
    {
        float fLength = sqrt(X * X + Y * Y);

        X /= fLength;
        Y /= fLength;
    }

    GEVector2 operator+(const GEVector2& v)
    {
        return GEVector2(X + v.X, Y + v.Y);
    }

    GEVector2& operator+=(const GEVector2& v)
    {
        X += v.X; Y += v.Y;
        return *this;
    }

    GEVector2 operator-(const GEVector2& v)
    {
        return GEVector2(X - v.X, Y - v.Y);
    }

    GEVector2& operator-=(const GEVector2& v)
    {
        X -= v.X; Y -= v.Y;
        return *this;
    }

    GEVector2 operator*(const float fValue)
    {
        return GEVector2(X * fValue, Y * fValue);
    }

    GEVector2& operator*=(const float fValue)
    {
        X *= fValue; Y *= fValue;
        return *this;
    }

    float operator*(const GEVector2& v)
    {
        return (X * v.X + Y * v.Y);
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

    float getLength() const
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

    float dotProduct(const GEVector3& other) const
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    GEVector3 crossProduct(const GEVector3& other) const
    {
        return GEVector3(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);
    }

    float getSquaredDistanceTo(const GEVector3& other) const
    {
        float fDeltaX = other.X - X;
        float fDeltaY = other.Y - Y;
        float fDeltaZ = other.Z - Z;

        return fDeltaX * fDeltaX + fDeltaY * fDeltaY + fDeltaZ * fDeltaZ;
    }

    float getDistanceTo(const GEVector3& other) const
    {
        float fDeltaX = other.X - X;
        float fDeltaY = other.Y - Y;
        float fDeltaZ = other.Z - Z;

        return sqrt(fDeltaX * fDeltaX + fDeltaY * fDeltaY + fDeltaZ * fDeltaZ);
    }

    void rotateYaw(float angle)
    {
        X = X * cosf(angle) - Z * sinf(angle);
        Z = X * sinf(angle) + Z * cosf(angle);
    }

    GEVector3 operator+(const GEVector3& v) const
    {
        return GEVector3(X + v.X, Y + v.Y, Z + v.Z);
    }

    GEVector3& operator+=(const GEVector3& v)
    {
        X += v.X; Y += v.Y; Z += v.Z;
        return *this;
    }

    GEVector3 operator-(const GEVector3& v) const
    {
        return GEVector3(X - v.X, Y - v.Y, Z - v.Z);
    }

    GEVector3& operator-=(const GEVector3& v)
    {
        X -= v.X; Y -= v.Y; Z -= v.Z;
        return *this;
    }

    GEVector3 operator*(const float fValue) const
    {
        return GEVector3(X * fValue, Y * fValue, Z * fValue);
    }

    GEVector3& operator*=(const float fValue)
    {
        X *= fValue; Y *= fValue; Z *= fValue;
        return *this;
    }

    float operator*(const GEVector3& v) const
    {
        return (X * v.X + Y * v.Y + Z * v.Z);
    }
};

struct GEMatrix3
{
    float m[12];
};
   
struct GEMatrix4
{
    float m[16];
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

   
//
//  Vectors
//
inline GEVector3 GEVector3Negate(const GEVector3& V)
{
   return GEVector3(-V.X, -V.Y, -V.Z);
}

inline GEVector3 GEVector3Add(const GEVector3& V1, const GEVector3& V2)
{
   return GEVector3(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

inline float GEVector3Length(const GEVector3& V)
{
   return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

inline GEVector3 GEVector3Normalize(const GEVector3& V)
{
   GEVector3 vNormalized(V);
   vNormalized.normalize();
   return vNormalized;
}

inline float GEVector3DotProduct(const GEVector3& V1, const GEVector3& V2)
{
   return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

inline GEVector3 GEVector3CrossProduct(const GEVector3& V1, const GEVector3& V2)
{
   return GEVector3(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}


   
//
//  Matrix generation
//
inline void GEMatrix4GetMatrix3(const GEMatrix4& M, GEMatrix3* Out)
{
   Out->m[0] = M.m[0];
   Out->m[1] = M.m[1];
   Out->m[2] = M.m[2];
   
   Out->m[3] = M.m[4];
   Out->m[4] = M.m[5];
   Out->m[5] = M.m[6];
   
   Out->m[6] = M.m[8];
   Out->m[7] = M.m[9];
   Out->m[8] = M.m[10];
}

inline void GEMatrix4Multiply(const GEMatrix4& M1, const GEMatrix4& M2, GEMatrix4* Out)
{
   Out->m[0]  = M1.m[0] * M2.m[0]  + M1.m[4] * M2.m[1]  + M1.m[8] * M2.m[2]   + M1.m[12] * M2.m[3];
   Out->m[4]  = M1.m[0] * M2.m[4]  + M1.m[4] * M2.m[5]  + M1.m[8] * M2.m[6]   + M1.m[12] * M2.m[7];
   Out->m[8]  = M1.m[0] * M2.m[8]  + M1.m[4] * M2.m[9]  + M1.m[8] * M2.m[10]  + M1.m[12] * M2.m[11];
   Out->m[12] = M1.m[0] * M2.m[12] + M1.m[4] * M2.m[13] + M1.m[8] * M2.m[14]  + M1.m[12] * M2.m[15];
   
   Out->m[1]  = M1.m[1] * M2.m[0]  + M1.m[5] * M2.m[1]  + M1.m[9] * M2.m[2]   + M1.m[13] * M2.m[3];
   Out->m[5]  = M1.m[1] * M2.m[4]  + M1.m[5] * M2.m[5]  + M1.m[9] * M2.m[6]   + M1.m[13] * M2.m[7];
   Out->m[9]  = M1.m[1] * M2.m[8]  + M1.m[5] * M2.m[9]  + M1.m[9] * M2.m[10]  + M1.m[13] * M2.m[11];
   Out->m[13] = M1.m[1] * M2.m[12] + M1.m[5] * M2.m[13] + M1.m[9] * M2.m[14]  + M1.m[13] * M2.m[15];
   
   Out->m[2]  = M1.m[2] * M2.m[0]  + M1.m[6] * M2.m[1]  + M1.m[10] * M2.m[2]  + M1.m[14] * M2.m[3];
   Out->m[6]  = M1.m[2] * M2.m[4]  + M1.m[6] * M2.m[5]  + M1.m[10] * M2.m[6]  + M1.m[14] * M2.m[7];
   Out->m[10] = M1.m[2] * M2.m[8]  + M1.m[6] * M2.m[9]  + M1.m[10] * M2.m[10] + M1.m[14] * M2.m[11];
   Out->m[14] = M1.m[2] * M2.m[12] + M1.m[6] * M2.m[13] + M1.m[10] * M2.m[14] + M1.m[14] * M2.m[15];
   
   Out->m[3]  = M1.m[3] * M2.m[0]  + M1.m[7] * M2.m[1]  + M1.m[11] * M2.m[2]  + M1.m[15] * M2.m[3];
   Out->m[7]  = M1.m[3] * M2.m[4]  + M1.m[7] * M2.m[5]  + M1.m[11] * M2.m[6]  + M1.m[15] * M2.m[7];
   Out->m[11] = M1.m[3] * M2.m[8]  + M1.m[7] * M2.m[9]  + M1.m[11] * M2.m[10] + M1.m[15] * M2.m[11];
   Out->m[15] = M1.m[3] * M2.m[12] + M1.m[7] * M2.m[13] + M1.m[11] * M2.m[14] + M1.m[15] * M2.m[15];
}

inline void GEMatrix4MakeIdentity(GEMatrix4* Out)
{
   Out->m[0] = 1.0f;
   Out->m[1] = 0.0f;
   Out->m[2] = 0.0f;
   Out->m[3] = 0.0f;
   
   Out->m[4] = 0.0f;
   Out->m[5] = 1.0f;
   Out->m[6] = 0.0f;
   Out->m[7] = 0.0f;
   
   Out->m[8] = 0.0f;
   Out->m[9] = 0.0f;
   Out->m[10] = 1.0f;
   Out->m[11] = 0.0f;
   
   Out->m[12] = 0.0f;
   Out->m[13] = 0.0f;
   Out->m[14] = 0.0f;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeTranslation(const GEVector3& Vector, GEMatrix4* Out)
{
   Out->m[0] = 1.0f;
   Out->m[1] = 0.0f;
   Out->m[2] = 0.0f;
   Out->m[3] = 0.0f;
   
   Out->m[4] = 0.0f;
   Out->m[5] = 1.0f;
   Out->m[6] = 0.0f;
   Out->m[7] = 0.0f;
   
   Out->m[8] = 0.0f;
   Out->m[9] = 0.0f;
   Out->m[10] = 1.0f;
   Out->m[11] = 0.0f;
   
   Out->m[12] = Vector.X;
   Out->m[13] = Vector.Y;
   Out->m[14] = Vector.Z;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeXRotation(float AngleInRadians, GEMatrix4* Out)
{
   float cos = cosf(AngleInRadians);
   float sin = sinf(AngleInRadians);
   
   Out->m[0] = 1.0f;
   Out->m[1] = 0.0f;
   Out->m[2] = 0.0f;
   Out->m[3] = 0.0f;
   
   Out->m[4] = 0.0f;
   Out->m[5] = cos;
   Out->m[6] = sin;
   Out->m[7] = 0.0f;
   
   Out->m[8] = 0.0f;
   Out->m[9] = -sin;
   Out->m[10] = cos;
   Out->m[11] = 0.0f;
   
   Out->m[12] = 0.0f;
   Out->m[13] = 0.0f;
   Out->m[14] = 0.0f;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeYRotation(float AngleInRadians, GEMatrix4* Out)
{
   float cos = cosf(AngleInRadians);
   float sin = sinf(AngleInRadians);
   
   Out->m[0] = cos;
   Out->m[1] = 0.0f;
   Out->m[2] = -sin;
   Out->m[3] = 0.0f;
   
   Out->m[4] = 0.0f;
   Out->m[5] = 1.0f;
   Out->m[6] = 0.0f;
   Out->m[7] = 0.0f;
   
   Out->m[8] = sin;
   Out->m[9] = 0.0f;
   Out->m[10] = cos;
   Out->m[11] = 0.0f;
   
   Out->m[12] = 0.0f;
   Out->m[13] = 0.0f;
   Out->m[14] = 0.0f;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeZRotation(float AngleInRadians, GEMatrix4* Out)
{
   float cos = cosf(AngleInRadians);
   float sin = sinf(AngleInRadians);
   
   Out->m[0] = cos;
   Out->m[1] = sin;
   Out->m[2] = 0.0f;
   Out->m[3] = 0.0f;
   
   Out->m[4] = -sin;
   Out->m[5] = cos;
   Out->m[6] = 0.0f;
   Out->m[7] = 0.0f;
   
   Out->m[8] = 0.0f;
   Out->m[9] = 0.0f;
   Out->m[10] = 1.0f;
   Out->m[11] = 0.0f;
   
   Out->m[12] = 0.0f;
   Out->m[13] = 0.0f;
   Out->m[14] = 0.0f;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeRotation(const GEVector3& Axis, float AngleInRadians, GEMatrix4* Out)
{
   GEVector3 v = GEVector3Normalize(Axis);
   float cos = cosf(AngleInRadians);
   float cosp = 1.0f - cos;
   float sin = sinf(AngleInRadians);
   
   Out->m[0] = cos + cosp * v.X * v.X;
   Out->m[1] = cosp * v.X * v.Y + v.Z * sin;
   Out->m[2] = cosp * v.X * v.Z - v.Y * sin;
   Out->m[3] = 0.0f;
   
   Out->m[4] = cosp * v.X * v.Y - v.Z * sin;
   Out->m[5] = cos + cosp * v.Y * v.Y;
   Out->m[6] = cosp * v.Y * v.Z + v.X * sin;
   Out->m[7] = 0.0f;
   
   Out->m[8] = cosp * v.X * v.Z + v.Y * sin;
   Out->m[9] = cosp * v.Y * v.Z - v.X * sin;
   Out->m[10] = cos + cosp * v.Z * v.Z;
   Out->m[11] = 0.0f;
   
   Out->m[12] = 0.0f;
   Out->m[13] = 0.0f;
   Out->m[14] = 0.0f;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeLookAt(const GEVector3& Eye, const GEVector3& Point, const GEVector3& Up, GEMatrix4* Out)
{
   GEVector3 n = GEVector3Normalize(GEVector3Add(Eye, GEVector3Negate(Point)));
   GEVector3 u = GEVector3Normalize(GEVector3CrossProduct(Up, n));
   GEVector3 v = GEVector3CrossProduct(n, u);
   
   Out->m[0] = u.X;
   Out->m[1] = v.X;
   Out->m[2] = n.X;
   Out->m[3] = 0.0f;
   
   Out->m[4] = u.Y;
   Out->m[5] = v.Y;
   Out->m[6] = n.Y;
   Out->m[7] = 0.0f;
   
   Out->m[8] = u.Z;
   Out->m[9] = v.Z;
   Out->m[10] = n.Z;
   Out->m[11] = 0.0f;
   
   Out->m[12] = GEVector3DotProduct(GEVector3Negate(u), Eye);
   Out->m[13] = GEVector3DotProduct(GEVector3Negate(v), Eye);
   Out->m[14] = GEVector3DotProduct(GEVector3Negate(n), Eye);
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakeOrtho(float Left, float Right, float Bottom, float Top, float Near, float Far, GEMatrix4* Out)
{
   float ral = Right + Left;
   float rsl = Right - Left;
   float tab = Top + Bottom;
   float tsb = Top - Bottom;
   float fan = Far + Near;
   float fsn = Far - Near;
   
   Out->m[0] = 2.0f / rsl;
   Out->m[1] = 0.0f;
   Out->m[2] = 0.0f;
   Out->m[3] = 0.0f;
   
   Out->m[4] = 0.0f;
   Out->m[5] = 2.0f / tsb;
   Out->m[6] = 0.0f;
   Out->m[7] = 0.0f;
   
   Out->m[8] = 0.0f;
   Out->m[9] = 0.0f;
   Out->m[10] = -2.0f / fsn;
   Out->m[11] = 0.0f;
   
   Out->m[12] = -ral / rsl;
   Out->m[13] = -tab / tsb;
   Out->m[14] = -fan / fsn;
   Out->m[15] = 1.0f;
}

inline void GEMatrix4MakePerspective(float FOVyRadians, float Aspect, float NearZ, float FarZ, GEMatrix4* Out)
{
   float cotan = 1.0f / tanf(FOVyRadians / 2.0f);
   
   Out->m[0] = cotan / Aspect;
   Out->m[1] = 0.0f;
   Out->m[2] = 0.0f;
   Out->m[3] = 0.0f;
   
   Out->m[4] = 0.0f;
   Out->m[5] = cotan;
   Out->m[6] = 0.0f;
   Out->m[7] = 0.0f;
   
   Out->m[8] = 0.0f;
   Out->m[9] = 0.0f;
   Out->m[10] = (FarZ + NearZ) / (NearZ - FarZ);
   Out->m[11] = -1.0f;
   
   Out->m[12] = 0.0f;
   Out->m[13] = 0.0f;
   Out->m[14] = (2.0f * FarZ * NearZ) / (NearZ - FarZ);
   Out->m[15] = 0.0f;
}


//
//  Matrix transformation
//
inline void GEMatrix4Scale(GEMatrix4* Matrix, const GEVector3& Scale)
{
   Matrix->m[0] *= Scale.X;
   Matrix->m[1] *= Scale.X;
   Matrix->m[2] *= Scale.X;
   Matrix->m[3] *= Scale.X;
   
   Matrix->m[4] *= Scale.Y;
   Matrix->m[5] *= Scale.Y;
   Matrix->m[6] *= Scale.Y;
   Matrix->m[7] *= Scale.Y;
   
   Matrix->m[8] *= Scale.Z;
   Matrix->m[9] *= Scale.Z;
   Matrix->m[10] *= Scale.Z;
   Matrix->m[11] *= Scale.Z;
}

inline void GEMatrix4RotateX(GEMatrix4* Matrix, float AngleInRadians)
{
   GEMatrix4 matRotation;
   GEMatrix4MakeXRotation(AngleInRadians, &matRotation);
   GEMatrix4 matCopy(*Matrix);
   GEMatrix4Multiply(matCopy, matRotation, Matrix);
}

inline void GEMatrix4RotateY(GEMatrix4* Matrix, float AngleInRadians)
{
   GEMatrix4 matRotation;
   GEMatrix4MakeYRotation(AngleInRadians, &matRotation);
   GEMatrix4 matCopy(*Matrix);
   GEMatrix4Multiply(matCopy, matRotation, Matrix);
}

inline void GEMatrix4RotateZ(GEMatrix4* Matrix, float AngleInRadians)
{
   GEMatrix4 matRotation;
   GEMatrix4MakeZRotation(AngleInRadians, &matRotation);
   GEMatrix4 matCopy(*Matrix);
   GEMatrix4Multiply(matCopy, matRotation, Matrix);
}

inline void GEMatrix3Transpose(GEMatrix3* Matrix)
{
   GEMatrix3 matCopy(*Matrix);
   
   Matrix->m[1] = matCopy.m[3];
   Matrix->m[2] = matCopy.m[6];
   
   Matrix->m[3] = matCopy.m[1];
   Matrix->m[5] = matCopy.m[7];
   
   Matrix->m[6] = matCopy.m[2];
   Matrix->m[7] = matCopy.m[5];
}

inline void GEMatrix3Invert(GEMatrix3* Matrix)
{
   GEMatrix3 matCopy(*Matrix);
   
   Matrix->m[0] = matCopy.m[4] * matCopy.m[8] - matCopy.m[5] * matCopy.m[7];
   Matrix->m[1] = matCopy.m[2] * matCopy.m[7] - matCopy.m[1] * matCopy.m[8];
   Matrix->m[2] = matCopy.m[1] * matCopy.m[5] - matCopy.m[2] * matCopy.m[4];
   
   Matrix->m[3] = matCopy.m[5] * matCopy.m[6] - matCopy.m[3] * matCopy.m[8];
   Matrix->m[4] = matCopy.m[0] * matCopy.m[8] - matCopy.m[2] * matCopy.m[6];
   Matrix->m[5] = matCopy.m[2] * matCopy.m[3] - matCopy.m[0] * matCopy.m[5];
   
   Matrix->m[6] = matCopy.m[3] * matCopy.m[7] - matCopy.m[4] * matCopy.m[6];
   Matrix->m[7] = matCopy.m[1] * matCopy.m[6] - matCopy.m[0] * matCopy.m[7];
   Matrix->m[8] = matCopy.m[0] * matCopy.m[4] - matCopy.m[1] * matCopy.m[3];
   
   float fDet = matCopy.m[0] * Matrix->m[0] + matCopy.m[1] * Matrix->m[3] + matCopy.m[2] * Matrix->m[6];
   
   if(fabs(fDet) <= 0.01f)
      return;
   
   float fInvDet = 1.0f / fDet;
   
   Matrix->m[0] *= fInvDet;
   Matrix->m[1] *= fInvDet;
   Matrix->m[2] *= fInvDet;
   Matrix->m[3] *= fInvDet;
   Matrix->m[4] *= fInvDet;
   Matrix->m[5] *= fInvDet;
   Matrix->m[6] *= fInvDet;
   Matrix->m[7] *= fInvDet;
   Matrix->m[8] *= fInvDet;
}
