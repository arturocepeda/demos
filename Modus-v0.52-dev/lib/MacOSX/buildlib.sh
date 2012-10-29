
cp ./../../src/*.* ./
cp ./../../include/*.* ./

g++ -w -c *.cpp
ar rvs modus.a *.o

rm *.o
rm *.cpp
rm *.h

