
/*
    Arturo Cepeda Pérez

    CRand - Random Generators

    --- CRand.cpp ---
*/

#include "CRand.h"
#include <cstdlib>


//
//  CRand
//
void CRand::setSeed(unsigned int Seed)
{
    srand(Seed);
}

float CRand::getBetween0and1()
{
    return (rand() / float(RAND_MAX));
}



//
//  CRandInt
//
CRandInt::CRandInt(int MinValue, int MaxValue)
{
    if(MinValue < MaxValue)
    {
        iMin = MinValue;
        iMax = MaxValue;
    }
    else
    {
        iMin = MaxValue;
        iMax = MinValue;
    }
}

int CRandInt::generate()
{
    return (rand() % (iMax - iMin + 1) + iMin);
}



//
//  CRandEvent
//
CRandEvent::CRandEvent(int Percentage)
{
    iPercentage = Percentage;
}

bool CRandEvent::occurs()
{
    return ((rand() % 100) < iPercentage);
}



//
//  CRandDie
//
int CRandDie::roll()
{
    return (rand() % 6 + 1);
}



//
//  CRandUniform
//
void CRandUniform::addNumber(int Number)
{
    iValues.push_back(Number);
}

int CRandUniform::extractNumber()
{
    return (iValues[rand() % iValues.size()]);
}

unsigned int CRandUniform::getSize()
{
    return iValues.size();
}



//
//  CRandUrn
//
int CRandUrn::extractNumber()
{
    int iPosition = rand() % iValues.size();
    int iValue = iValues[iPosition];

    iValues.erase(iValues.begin() + iPosition);

    return iValue;
}



//
//  CRandExponential
//
CRandExponential::CRandExponential(float Lambda)
{
    fLambda = Lambda;
}

float CRandExponential::generate()
{
    float fValue = 0.0f;

    while(fValue == 0.0f)
        fValue = getBetween0and1();

    return (-log(fValue) / fLambda);
}



//
//  CRandNormal
//
CRandNormal::CRandNormal(float Mean, float StandardDeviation)
{
    fMean = Mean;
    fStandardDeviation = StandardDeviation;

    // constants
    iN = 12;
    fHalfN = 6.0f;
    fScale = 1.0f;
}

float CRandNormal::generate()
{
    float fSum = 0.0f;

    for(int i = 0; i < iN; i++)
        fSum += getBetween0and1();

    return (fStandardDeviation * fScale * (fSum - fHalfN) + fMean);
}
