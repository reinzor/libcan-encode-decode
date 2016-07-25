# libcan-encode-decode

[![Build Status](https://travis-ci.org/reinzor/libcan-encode-decode.svg?branch=master)](https://travis-ci.org/reinzor/libcan-encode-decode)

C++ functions for encoding and decoding CAN frames ( uint8_t[8] )

## Usage

Copy the include/can_encode_decode.inl into your c++ project and use the inline functions.

    uint8_t src_array[8];

    encode(src_array, 3.0, 56, 3, false, false, 1.000000, 0);
    decode(src_array, 56, 3, false, false, 1.000000, 0); // 3.0

    encode(src_array, 35, 2, 6, true, false, 1.0, 0);
    decode(src_array, 2, 6, true, false, 1.000000, 0) // 35.000000

    encode(src_array, 1.0, 0, 2, true, false, 1.0, 0);
    decode(src_array, 0, 2, true, false, 1.000000, 0); // 1.000000

## Run tests

`./run_tests.sh`
