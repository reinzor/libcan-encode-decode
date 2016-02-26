#include "../include/can_encode_decode_inl.h"

#include <iostream>
#include <stdlib.h> 

#include <stack>
#include <ctime>

std::stack<clock_t> tictoc_stack;
unsigned int num_tests = 0;

void tic() 
{
    tictoc_stack.push(clock());
}

void toc() 
{
    std::cout << "All " << num_tests << " tests passed within " << ((double)(clock() - tictoc_stack.top())) / CLOCKS_PER_SEC << " seconds!" << std::endl;
    tictoc_stack.pop();
}

inline void TEST(float k, float v)
{
    num_tests++;
    bool near = (k >= v-0.01 && k <= v+0.01);
    if(!near)
    {
        std::cerr << "Expected " << v << " but got " << k << std::endl;
        exit(1);
    }
}

int main()
{
    tic();

    // Decode test 1 (big endian unsigned)
    {
        uint64_t src = 0x182000110008d;
        uint8_t src_array[8] = {141, 0, 16, 1, 0, 130, 1, 0};

        TEST(extractSignal(src_array, 0, 2, true, false), 1);
        TEST(decode(src_array, 0, 2, true, false, 1.000000, 0), 1.000000);

        TEST(extractSignal(src_array, 2, 6, true, false), 35);
        TEST(decode(src_array, 2, 6, true, false, 1.000000, 0), 35.000000);

        TEST(extractSignal(src_array, 21, 11, true, false), 0);
        TEST(decode(src_array, 21, 11, true, false, 0.100000, 0), 0.000000);

        TEST(extractSignal(src_array, 25, 12, true, false), 2048);
        TEST(decode(src_array, 25, 12, true, false, 0.062500, -128), 0.000000);

        TEST(extractSignal(src_array, 32, 9, true, false), 256);
        TEST(decode(src_array, 32, 9, true, false, 0.062500, -16), 0.000000);

        TEST(extractSignal(src_array, 48, 3, true, false), 1);
        TEST(decode(src_array, 48, 3, true, false, 1.000000, 0), 1.000000);

        TEST(extractSignal(src_array, 51, 3, true, false), 0);
        TEST(decode(src_array, 51, 3, true, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 54, 10, true, false), 520);
        TEST(decode(src_array, 54, 10, true, false, 0.100000, -52), 0.000000);

        TEST(extractSignal(src_array, 56, 3, true, false), 0);
        TEST(decode(src_array, 56, 3, true, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 59, 3, true, false), 0);
        TEST(decode(src_array, 59, 3, true, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 62, 2, true, false), 0);
        TEST(decode(src_array, 62, 2, true, false, 1.000000, 0), 0.000000);
    }

    // Decode test 2 (little endian, signed and unsigned)
    {
        // src, startbit, bitlength, is_big_endian, is_signed, factor, offset, name, value
        uint8_t src_array[8] = {12, 0, 5, 112, 3, 205, 31, 131};

        TEST(extractSignal(src_array, 60, 2, false, false), 0);
        TEST(decode(src_array, 60, 2, false, true, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 55, 1, false, false), 0);
        TEST(decode(src_array, 55, 1, false, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 20, 4, false, false), 0);
        TEST(decode(src_array, 20, 4, false, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 62, 2, false, false), 2);
        TEST(decode(src_array, 62, 2, false, false, 1.000000, 0), 2.000000);

        TEST(extractSignal(src_array, 34, 3, false, false), 0);
        TEST(decode(src_array, 34, 3, false, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 37, 3, false, false), 0);
        TEST(decode(src_array, 37, 3, false, false, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 59, 1, false, true), 0);
        TEST(decode(src_array, 59, 1, false, true, 1.000000, 0), 0.000000);

        TEST(extractSignal(src_array, 56, 3, false, false), 3);
        TEST(decode(src_array, 56, 3, false, false, 1.000000, 0), 3.000000);

        TEST(extractSignal(src_array, 52, 3, false, false), 1);
        TEST(decode(src_array, 52, 3, false, false, 1.000000, 0), 1.000000);

        TEST(extractSignal(src_array, 8, 12, false, false), 1280);
        TEST(decode(src_array, 8, 12, false, false, 0.062500, 0), 80.000000);

        TEST(decode(src_array, 40, 12, false, true, 0.062500, 0), -3.187500);

        TEST(extractSignal(src_array, 24, 10, false, true), (uint64_t) -144);
        TEST(decode(src_array, 24, 10, false, true, 0.062500, 0), -9.000000);

        TEST(extractSignal(src_array, 0, 8, false, false), 12);
        TEST(decode(src_array, 0, 8, false, false, 1.000000, 0), 12.000000);
    }

    toc();
}

