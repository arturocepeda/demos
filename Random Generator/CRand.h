
/*
    Arturo Cepeda Pérez

    CRand - Random Generators

    --- CRand.h ---
*/

#ifndef _CRAND_H_
#define _CRAND_H_

#include <vector>


//
//  CRand
//
class CRand
{
protected:
    float getBetween0and1();

public:
    void setSeed(unsigned int Seed);
};



//
//  CRandInt
//
class CRandInt : public CRand
{
private:
    int iMin;
    int iMax;

public:
    CRandInt(int MinValue, int MaxValue);
    int generate();
};



//
//  CRandEvent
//
class CRandEvent : public CRand
{
private:
    int iPercentage;

public:
    CRandEvent(int Percentage);
    bool occurs();
};



//
//  CRandDie
//
class CRandDie : public CRand
{
public:
    int roll();
};



//
//  CRandUniform
//
class CRandUniform : public CRand
{
protected:
    std::vector<int> iValues;

public:
    void addNumber(int Number);
    int extractNumber();
    unsigned int getSize();
};



//
//  CRandUrn
//
class CRandUrn : public CRandUniform
{
public:
    int extractNumber();
};



//
//  CRandExponential
//
class CRandExponential : public CRand
{
private:
    float fLambda;

public:
    CRandExponential(float Lambda);
    float generate();
};



//
//  CRandNormal
//
class CRandNormal : public CRand
{
private:
    float fMean;
    float fStandardDeviation;

    int iN;
    float fHalfN;
    float fScale;

public:
    CRandNormal(float Mean, float StandardDeviation);
    float generate();
};

#endif
