clang++ -std=c++0x parser.cc -o go.exe -I. -fno-rtti -fno-exceptions -Wall
clang++ -std=c++0x parser.cc -S -o go.asm -I. -fno-rtti -O3 -fverbose-asm