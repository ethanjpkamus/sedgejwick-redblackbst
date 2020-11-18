clear

echo "rm *.o"
rm *.o

echo "clang++ -std=c++17 -Wall sqb.cpp -o sqb.o"
clang++  -std=c++17 -Wall sqb.cpp -o sqb.o

echo "./sqb.o"
./sqb.o
