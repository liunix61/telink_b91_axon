/**
 *          Copyright (c) 2020-2022, Atlazo Inc.
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 */
#include <stdint.h>
#include "axon_demo_private.h"


/*
 * matrix multiply 8 bit
 */
int8_t _Alignas(16) matrix_mult_input_x8[MATRIX_MULT_8BIT_VECTOR_LENGTH] = {
    -10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,0,0,0,
};

/*
 * matrix multiply "y" can be stored in flash as driver
 * will copy it to ram as needed.
 */
const int8_t matrix_mult_input_y8[MATRIX_MULT_8BIT_MATRIX_HEIGHT][MATRIX_MULT_8BIT_VECTOR_LENGTH] = {
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0, }, // ROW 0
    { 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0, }, // ROW 1
    { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0, }, // ROW 2
    { 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0, }, // rOW 3
    { 2,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0, }, // rOW 4
};
// 8 bit in and out
int8_t matrix_mult_output_q8[MATRIX_MULT_8BIT_MATRIX_HEIGHT];

const int8_t matrix_mult_expected_output8[MATRIX_MULT_8BIT_MATRIX_HEIGHT] = {
    -10, -1, -55, 45, 25,
};
//8 bit in and 32 bit out
int32_t matrix_mult_output_q32[MATRIX_MULT_8BIT_MATRIX_HEIGHT];

const int32_t matrix_mult_expected_output32[MATRIX_MULT_8BIT_MATRIX_HEIGHT] = {
    -10, -1, -55, 45, 25,
};
/*
 * matrix multiply 16 bit
 */

int16_t _Alignas(8) matrix_mult_input_x[MATRIX_MULT_VECTOR_LENGTH] = {
    -10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,0,0,0,0,
};

/*
 * matrix multiply "y" can be stored in flash as driver
 * will copy it to ram as needed.
 */
const int16_t matrix_mult_input_y[MATRIX_MULT_MATRIX_HEIGHT][MATRIX_MULT_VECTOR_LENGTH] = {
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0, }, // ROW 0
    { 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0, }, // ROW 1
    { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0, }, // ROW 2
    { 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0, }, // rOW 3
    { 2,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0, }, // rOW 4
    { 1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0, }, // rOW 5
};

int16_t matrix_mult_output_q[MATRIX_MULT_MATRIX_HEIGHT];

const int16_t matrix_mult_expected_output[MATRIX_MULT_MATRIX_HEIGHT] = {
    -10, -1, -55, 45, 25, 35,
};

const int16_t matrix_mult_sigmoid_expected_output[MATRIX_MULT_MATRIX_HEIGHT] = {
    126, 128, 115, 139, 134, 136,
};

const int16_t matrix_mult_tanh_expected_output[MATRIX_MULT_MATRIX_HEIGHT] = {
    -10, 0, -54, 44, 24, 34,
};

int8_t _Alignas(16) memcpy_in[MEMCPY_VECTOR_LENGTH] = {
    -10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,
};

int8_t _Alignas(16) memcpy_out[MEMCPY_VECTOR_LENGTH];

/*
 * FIR vectors
 */
int32_t fir_outputs[FIR_DATA_LENGTH];

int32_t fir_input_x[FIR_DATA_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,7};

int32_t fir_input_F[FIR_FILTER_LENGTH] = {2,2,2,2,0,0,0,0,0,0,0,0};

const int32_t fir_expected_outputs[FIR_DATA_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,2,6,12,20,28,36,44,50};


/*
 * Run FIR with fir_rounded_input_x and fir_input_F keeps it below 32 bits.
 * Rounding by 20 should get the same result as unrounded FIR (except for last value which is "7" instead of "8")
 */

/*
 * SQRT vectors
 */
int32_t sqrt_input_x[SQRT_EXP_LGN_DATA_LENGTH] = {
    1 << 8,  // 1/16 = .25^2
    1 << 10,  // .25 = .5^2
    1 << 12, // 1.0 = 1^2
    4 << 12, // 4.0 = 2^2
    9 << 12, // 9.0 = 3^2
    16 << 12,// 16 = 4^2
    25 << 12,  // 25 = 5^2
    100 << 12 // 100 = 10^2
};

int32_t sqrt_outputs[SQRT_EXP_LGN_DATA_LENGTH];

/**
 * SQRT expected results
 */
const int32_t sqrt_expected_outputs[SQRT_EXP_LGN_DATA_LENGTH] = {
    1 << 4, // .25
    1 << 5, // .5
    1 << 6, // 1
    2 << 6, // 2
    3 << 6, // 3
    4 << 6,// 4
    5 << 6,  // 5
    10 << 6 // 10
};

/*
 * EXP vectors
 */

int32_t exp_input_x[SQRT_EXP_LGN_DATA_LENGTH] = {
    0 << 12,
    1 << 12,
    2 << 12,
    3 << 12,
    4 << 12,
    5 << 12,
    6 << 12,
    7 << 12 };

int32_t exp_outputs[SQRT_EXP_LGN_DATA_LENGTH];


/*
 * These will be used as inputs to logn so cannot be stored in FLASH.
 */
int32_t exp_expected_outputs[SQRT_EXP_LGN_DATA_LENGTH] = {
    0x1000,
    0x2b7c,
    0x7639,
    0x14168,
    0x369ea,
    0x94650,
    0x19371c,
    0x448c40
};

/*
 * For logn, the exp expected values will be the input, and the exp input will
 * be the expected outputs
 */
int32_t logn_outputs[SQRT_EXP_LGN_DATA_LENGTH];


/*
 * All XY vector operations will use the same input vectors
 */
int32_t xy_vector_op_x_in[XY_VECTOR_OPS_DATA_LENGTH] = {
    0,     1,   2,   3,   4,   5,   6,   7 };

int32_t xy_vector_op_y_in[XY_VECTOR_OPS_DATA_LENGTH] = {
    100, 101, 102, 103, 104, 105, 106, 107 };
// Xpy

const int32_t xy_vector_op_xpy_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    100, 102, 104, 106, 108, 110, 112, 114 };
int32_t xy_vector_op_xpy_out[XY_VECTOR_OPS_DATA_LENGTH];

// xmy

const int32_t xy_vector_op_xmy_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    -100, -100, -100, -100, -100, -100, -100, -100 };
int32_t xy_vector_op_xmy_out[XY_VECTOR_OPS_DATA_LENGTH];

// xspys

const int32_t xy_vector_op_xspys_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    10000, 10202, 10408, 10618, 10832, 11050, 11272, 11498 };
int32_t xy_vector_op_xspys_out[XY_VECTOR_OPS_DATA_LENGTH];

// xsmys

const int32_t xy_vector_op_xsmys_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    -10000, -10200, -10400, -10600, -10800, -11000, -11200, -11400 };
int32_t xy_vector_op_xsmys_out[XY_VECTOR_OPS_DATA_LENGTH];

// xty

const int32_t xy_vector_op_xty_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    0, 101, 204, 309, 416, 525, 636, 749 };
int32_t xy_vector_op_xty_out[XY_VECTOR_OPS_DATA_LENGTH];

// xty stride=2 on x and q

const int32_t xy_vector_op_xty_stride2_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    0, 0, 202, 0, 408, 0, 618, 0 };
int32_t xy_vector_op_xty_stride2_out[XY_VECTOR_OPS_DATA_LENGTH];

// axpby

int32_t axpby_a_in = 3;

int32_t axpby_b_in = 7;

const int32_t xy_vector_op_axpby_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    700, 710, 720, 730, 740, 750, 760, 770 };
int32_t xy_vector_op_axpby_out[XY_VECTOR_OPS_DATA_LENGTH];

// axpb will use same a & b as axpb

const int32_t xy_vector_op_axpb_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    7, 10, 13, 16, 19, 22, 25, 28};
int32_t xy_vector_op_axpb_out[XY_VECTOR_OPS_DATA_LENGTH];

// xs
const int32_t xy_vector_op_xs_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    0, 1, 4, 9, 16, 25, 36, 49};
int32_t xy_vector_op_xs_out[XY_VECTOR_OPS_DATA_LENGTH];

// relu.

int32_t xy_vector_op_relu_in[XY_VECTOR_OPS_DATA_LENGTH] = {
    -1, 10, -15, 100, 75, -20000, 30000, 1523 };

const int32_t xy_vector_op_relu_expected_out[XY_VECTOR_OPS_DATA_LENGTH] = {
    0, 10, 0, 100, 75, 0, 30000, 1523 };
int32_t xy_vector_op_relu_out[XY_VECTOR_OPS_DATA_LENGTH];

/*
 * Auto-correlation test vectors
 */
int32_t acorr_delay = ACORR_DELAY;

int32_t acorr_input_x[ACORR_VECTOR_LENGTH] = {5,4,3,2,1,0,0,0};

const int32_t acorr_expected_out[ACORR_DELAY] = {55,40,26,14,5,0};
int32_t acorr_out[ACORR_DELAY];


/*
 * Mar test vectors.
 * Will use xy_vector_op_x_in, y_in
 */
int32_t mar_expected_out = 2940;
int32_t mar_out;

/*
 * Acc test vectors.
 * Will use xy_vector_op_x_in, y_in
 */
int32_t acc_expected_out = 28;
int32_t acc_out;

/*
 * L2norm test vectors.
 * Will use xy_vector_op_x_in, y_in
 */
int32_t l2norm_expected_out = 140;
int32_t l2norm_out;


/*
 * FFT 512 vectors
 */
int32_t fft_outputs[1024];


const int32_t fft_512_expected[1024] = {
0xffffeb7a,
0x00000000,
0xffffeb73,
0xffffff94,
0xffffeb5e,
0xffffff2a,
0xffffeb3d,
0xfffffebe,
0xffffeb0c,
0xfffffe54,
0xffffeacd,
0xfffffde9,
0xffffea7d,
0xfffffd81,
0xffffea1a,
0xfffffd19,
0xffffe9a3,
0xfffffcb1,
0xffffe914,
0xfffffc4e,
0xffffe86c,
0xfffffbeb,
0xffffe7a3,
0xfffffb8a,
0xffffe6b5,
0xfffffb2f,
0xffffe59b,
0xfffffad8,
0xffffe44a,
0xfffffa88,
0xffffe2b5,
0xfffffa40,
0xffffe0ca,
0xfffffa05,
0xffffde6e,
0xfffff9d9,
0xffffdb7c,
0xfffff9c2,
0xffffd7bb,
0xfffff9ca,
0xffffd2d0,
0xfffffa02,
0xffffcc28,
0xfffffa80,
0xffffc2b4,
0xfffffb70,
0xffffb44e,
0xfffffd2f,
0xffff9bf2,
0x00000094,
0xffff6a40,
0x0000082d,
0xfffecefc,
0x00002160,
0x00154bbc,
0xfffc5fe8,
0x00010f60,
0xffffbfb0,
0x00008ae2,
0xffffd503,
0x00005d72,
0xffffdbc7,
0x000046a2,
0xffffdebe,
0x00003905,
0xffffe023,
0x0000300e,
0xffffe0b8,
0x000029c6,
0xffffe0d2,
0x00002530,
0xffffe099,
0x000021bc,
0xffffe023,
0x00001f18,
0xffffdf80,
0x00001d0c,
0xffffdeb4,
0x00001b76,
0xffffddc6,
0x00001a3c,
0xffffdcba,
0x0000194e,
0xffffdb8e,
0x000018a0,
0xffffda46,
0x00001827,
0xffffd8de,
0x000017df,
0xffffd758,
0x000017c2,
0xffffd5b0,
0x000017ce,
0xffffd3e5,
0x000017fe,
0xffffd1f3,
0x00001856,
0xffffcfd6,
0x000018d0,
0xffffcd8a,
0x00001972,
0xffffcb0a,
0x00001a3a,
0xffffc850,
0x00001b2e,
0xffffc553,
0x00001c4d,
0xffffc20a,
0x00001da0,
0xffffbe68,
0x00001f2a,
0xffffba61,
0x000020f4,
0xffffb5e0,
0x00002307,
0xffffb0d4,
0x00002571,
0xffffab1d,
0x00002842,
0xffffa49a,
0x00002b92,
0xffff9d1a,
0x00002f7c,
0xffff9462,
0x00003429,
0xffff8a18,
0x000039d2,
0xffff7dce,
0x000040c8,
0xffff6ed4,
0x00004984,
0xffff5c32,
0x000054c4,
0xffff445c,
0x000063be,
0xffff24ca,
0x000078a2,
0xfffef8f8,
0x000097b4,
0xfffeb806,
0x0000caa6,
0xfffe4dcc,
0x00012d50,
0xfffd8080,
0x00023cec,
0xfffb4c0c,
0x00122986,
0xffda3980,
0xfffcede6,
0x000651b6,
0xfffe8fd6,
0x0002ed6a,
0xffff0d21,
0x0001e8de,
0xffff4966,
0x00016b5f,
0xffff6cce,
0x00012187,
0xffff8416,
0x0000f0de,
0xffff948c,
0x0000ce62,
0xffffa0cc,
0x0000b4a8,
0xffffaa41,
0x0000a0b9,
0xffffb1c7,
0x000090d2,
0xffffb7e8,
0x000083d4,
0xffffbcfc,
0x00007904,
0xffffc142,
0x00006fe0,
0xffffc4e8,
0x0000680c,
0xffffc80e,
0x00006142,
0xffffcacc,
0x00005b52,
0xffffcd36,
0x00005612,
0xffffcf58,
0x00005168,
0xffffd13f,
0x00004d3a,
0xffffd2f3,
0x00004976,
0xffffd47c,
0x0000460d,
0xffffd5e1,
0x000042f3,
0xffffd724,
0x0000401c,
0xffffd84c,
0x00003d80,
0xffffd95b,
0x00003b18,
0xffffda54,
0x000038e0,
0xffffdb3a,
0x000036d1,
0xffffdc0e,
0x000034e4,
0xffffdcd4,
0x0000331a,
0xffffdd8a,
0x0000316d,
0xffffde36,
0x00002fda,
0xffffded6,
0x00002e60,
0xffffdf6a,
0x00002cfc,
0xffffdff6,
0x00002bab,
0xffffe07a,
0x00002a6e,
0xffffe0f4,
0x00002940,
0xffffe168,
0x00002822,
0xffffe1d7,
0x00002712,
0xffffe23f,
0x00002610,
0xffffe2a0,
0x0000251a,
0xffffe2fe,
0x0000242f,
0xffffe356,
0x0000234d,
0xffffe3aa,
0x00002277,
0xffffe3fa,
0x000021a8,
0xffffe446,
0x000020e2,
0xffffe48e,
0x00002024,
0xffffe4d4,
0x00001f6e,
0xffffe514,
0x00001ec0,
0xffffe554,
0x00001e16,
0xffffe590,
0x00001d73,
0xffffe5c9,
0x00001cd6,
0xffffe600,
0x00001c3e,
0xffffe636,
0x00001bac,
0xffffe668,
0x00001b1e,
0xffffe698,
0x00001a95,
0xffffe6c7,
0x00001a10,
0xffffe6f4,
0x00001990,
0xffffe71f,
0x00001912,
0xffffe749,
0x0000189a,
0xffffe770,
0x00001823,
0xffffe796,
0x000017b0,
0xffffe7bc,
0x00001742,
0xffffe7e0,
0x000016d6,
0xffffe802,
0x0000166c,
0xffffe823,
0x00001606,
0xffffe843,
0x000015a2,
0xffffe862,
0x00001540,
0xffffe880,
0x000014e1,
0xffffe89e,
0x00001485,
0xffffe8ba,
0x0000142a,
0xffffe8d4,
0x000013d1,
0xffffe8ef,
0x0000137a,
0xffffe908,
0x00001326,
0xffffe920,
0x000012d2,
0xffffe938,
0x00001282,
0xffffe950,
0x00001233,
0xffffe966,
0x000011e4,
0xffffe97c,
0x00001199,
0xffffe990,
0x0000114e,
0xffffe9a4,
0x00001106,
0xffffe9b8,
0x000010be,
0xffffe9cc,
0x00001078,
0xffffe9de,
0x00001033,
0xffffe9f1,
0x00000fee,
0xffffea03,
0x00000fac,
0xffffea14,
0x00000f6a,
0xffffea24,
0x00000f2a,
0xffffea35,
0x00000eeb,
0xffffea44,
0x00000ead,
0xffffea53,
0x00000e70,
0xffffea62,
0x00000e34,
0xffffea71,
0x00000df8,
0xffffea7e,
0x00000dbe,
0xffffea8e,
0x00000d84,
0xffffea9a,
0x00000d4c,
0xffffeaa8,
0x00000d14,
0xffffeab4,
0x00000cdc,
0xffffeac0,
0x00000ca6,
0xffffeacc,
0x00000c72,
0xffffead8,
0x00000c3c,
0xffffeae3,
0x00000c09,
0xffffeaee,
0x00000bd5,
0xffffeaf9,
0x00000ba2,
0xffffeb03,
0x00000b70,
0xffffeb0e,
0x00000b3f,
0xffffeb18,
0x00000b0e,
0xffffeb21,
0x00000ade,
0xffffeb2b,
0x00000aae,
0xffffeb35,
0x00000a80,
0xffffeb3e,
0x00000a50,
0xffffeb46,
0x00000a22,
0xffffeb4e,
0x000009f4,
0xffffeb58,
0x000009c8,
0xffffeb60,
0x0000099b,
0xffffeb68,
0x00000970,
0xffffeb6e,
0x00000944,
0xffffeb76,
0x00000918,
0xffffeb7e,
0x000008ee,
0xffffeb85,
0x000008c4,
0xffffeb8c,
0x0000089a,
0xffffeb93,
0x00000870,
0xffffeb9a,
0x00000846,
0xffffeba0,
0x0000081d,
0xffffeba6,
0x000007f4,
0xffffebad,
0x000007cc,
0xffffebb2,
0x000007a4,
0xffffebb8,
0x0000077c,
0xffffebbe,
0x00000755,
0xffffebc4,
0x0000072e,
0xffffebc9,
0x00000708,
0xffffebce,
0x000006e1,
0xffffebd4,
0x000006bc,
0xffffebd8,
0x00000696,
0xffffebdd,
0x00000670,
0xffffebe2,
0x0000064b,
0xffffebe6,
0x00000626,
0xffffebea,
0x00000601,
0xffffebf0,
0x000005dc,
0xffffebf3,
0x000005b8,
0xffffebf7,
0x00000594,
0xffffebfc,
0x00000570,
0xffffec00,
0x0000054c,
0xffffec02,
0x00000528,
0xffffec06,
0x00000506,
0xffffec0a,
0x000004e2,
0xffffec0e,
0x000004c0,
0xffffec10,
0x0000049c,
0xffffec14,
0x0000047b,
0xffffec18,
0x00000458,
0xffffec1a,
0x00000435,
0xffffec1d,
0x00000413,
0xffffec20,
0x000003f2,
0xffffec22,
0x000003cf,
0xffffec24,
0x000003ad,
0xffffec26,
0x0000038c,
0xffffec2a,
0x0000036a,
0xffffec2c,
0x0000034a,
0xffffec2e,
0x00000328,
0xffffec30,
0x00000306,
0xffffec32,
0x000002e6,
0xffffec34,
0x000002c4,
0xffffec36,
0x000002a4,
0xffffec38,
0x00000283,
0xffffec39,
0x00000262,
0xffffec3a,
0x00000242,
0xffffec3c,
0x00000221,
0xffffec3e,
0x00000201,
0xffffec3f,
0x000001e0,
0xffffec40,
0x000001c0,
0xffffec41,
0x000001a0,
0xffffec43,
0x00000180,
0xffffec43,
0x00000160,
0xffffec44,
0x0000013f,
0xffffec45,
0x00000120,
0xffffec46,
0x000000ff,
0xffffec46,
0x000000df,
0xffffec47,
0x000000bf,
0xffffec47,
0x0000009f,
0xffffec48,
0x00000080,
0xffffec48,
0x00000060,
0xffffec48,
0x00000040,
0xffffec48,
0x00000020,
0xffffec48,
0x00000000,
0xffffec48,
0xffffffe0,
0xffffec48,
0xffffffc0,
0xffffec48,
0xffffffa0,
0xffffec48,
0xffffff80,
0xffffec47,
0xffffff61,
0xffffec47,
0xffffff41,
0xffffec46,
0xffffff21,
0xffffec46,
0xffffff01,
0xffffec45,
0xfffffee0,
0xffffec44,
0xfffffec1,
0xffffec43,
0xfffffea0,
0xffffec43,
0xfffffe80,
0xffffec41,
0xfffffe60,
0xffffec40,
0xfffffe40,
0xffffec3f,
0xfffffe20,
0xffffec3e,
0xfffffdff,
0xffffec3c,
0xfffffddf,
0xffffec3a,
0xfffffdbe,
0xffffec39,
0xfffffd9e,
0xffffec38,
0xfffffd7d,
0xffffec36,
0xfffffd5c,
0xffffec34,
0xfffffd3c,
0xffffec32,
0xfffffd1a,
0xffffec30,
0xfffffcfa,
0xffffec2e,
0xfffffcd8,
0xffffec2c,
0xfffffcb6,
0xffffec2a,
0xfffffc96,
0xffffec26,
0xfffffc74,
0xffffec24,
0xfffffc53,
0xffffec22,
0xfffffc31,
0xffffec20,
0xfffffc0e,
0xffffec1d,
0xfffffbed,
0xffffec1a,
0xfffffbcb,
0xffffec18,
0xfffffba8,
0xffffec14,
0xfffffb85,
0xffffec10,
0xfffffb64,
0xffffec0e,
0xfffffb40,
0xffffec0a,
0xfffffb1e,
0xffffec06,
0xfffffafa,
0xffffec02,
0xfffffad8,
0xffffec00,
0xfffffab4,
0xffffebfc,
0xfffffa90,
0xffffebf7,
0xfffffa6c,
0xffffebf3,
0xfffffa48,
0xffffebf0,
0xfffffa24,
0xffffebea,
0xfffff9ff,
0xffffebe6,
0xfffff9da,
0xffffebe2,
0xfffff9b5,
0xffffebdd,
0xfffff990,
0xffffebd8,
0xfffff96a,
0xffffebd4,
0xfffff944,
0xffffebce,
0xfffff91f,
0xffffebc9,
0xfffff8f8,
0xffffebc4,
0xfffff8d2,
0xffffebbe,
0xfffff8ab,
0xffffebb8,
0xfffff884,
0xffffebb2,
0xfffff85c,
0xffffebad,
0xfffff834,
0xffffeba6,
0xfffff80c,
0xffffeba0,
0xfffff7e3,
0xffffeb9a,
0xfffff7ba,
0xffffeb93,
0xfffff790,
0xffffeb8c,
0xfffff766,
0xffffeb85,
0xfffff73c,
0xffffeb7e,
0xfffff712,
0xffffeb76,
0xfffff6e8,
0xffffeb6e,
0xfffff6bc,
0xffffeb68,
0xfffff690,
0xffffeb60,
0xfffff665,
0xffffeb58,
0xfffff638,
0xffffeb4e,
0xfffff60c,
0xffffeb46,
0xfffff5de,
0xffffeb3e,
0xfffff5b0,
0xffffeb35,
0xfffff580,
0xffffeb2b,
0xfffff552,
0xffffeb21,
0xfffff522,
0xffffeb18,
0xfffff4f2,
0xffffeb0e,
0xfffff4c1,
0xffffeb03,
0xfffff490,
0xffffeaf9,
0xfffff45e,
0xffffeaee,
0xfffff42b,
0xffffeae3,
0xfffff3f7,
0xffffead8,
0xfffff3c4,
0xffffeacc,
0xfffff38e,
0xffffeac0,
0xfffff35a,
0xffffeab4,
0xfffff324,
0xffffeaa8,
0xfffff2ec,
0xffffea9a,
0xfffff2b4,
0xffffea8e,
0xfffff27c,
0xffffea7e,
0xfffff242,
0xffffea71,
0xfffff208,
0xffffea62,
0xfffff1cc,
0xffffea53,
0xfffff190,
0xffffea44,
0xfffff153,
0xffffea35,
0xfffff115,
0xffffea24,
0xfffff0d6,
0xffffea14,
0xfffff096,
0xffffea03,
0xfffff054,
0xffffe9f1,
0xfffff012,
0xffffe9de,
0xffffefcd,
0xffffe9cc,
0xffffef88,
0xffffe9b8,
0xffffef42,
0xffffe9a4,
0xffffeefa,
0xffffe990,
0xffffeeb2,
0xffffe97c,
0xffffee67,
0xffffe966,
0xffffee1c,
0xffffe950,
0xffffedcd,
0xffffe938,
0xffffed7e,
0xffffe920,
0xffffed2e,
0xffffe908,
0xffffecda,
0xffffe8ef,
0xffffec86,
0xffffe8d4,
0xffffec2f,
0xffffe8ba,
0xffffebd6,
0xffffe89e,
0xffffeb7b,
0xffffe880,
0xffffeb1f,
0xffffe862,
0xffffeac0,
0xffffe843,
0xffffea5e,
0xffffe823,
0xffffe9fa,
0xffffe802,
0xffffe994,
0xffffe7e0,
0xffffe92a,
0xffffe7bc,
0xffffe8be,
0xffffe796,
0xffffe850,
0xffffe770,
0xffffe7dd,
0xffffe749,
0xffffe766,
0xffffe71f,
0xffffe6ee,
0xffffe6f4,
0xffffe670,
0xffffe6c7,
0xffffe5f0,
0xffffe698,
0xffffe56b,
0xffffe668,
0xffffe4e2,
0xffffe636,
0xffffe454,
0xffffe600,
0xffffe3c2,
0xffffe5c9,
0xffffe32a,
0xffffe590,
0xffffe28d,
0xffffe554,
0xffffe1ea,
0xffffe514,
0xffffe140,
0xffffe4d4,
0xffffe092,
0xffffe48e,
0xffffdfdc,
0xffffe446,
0xffffdf1e,
0xffffe3fa,
0xffffde58,
0xffffe3aa,
0xffffdd89,
0xffffe356,
0xffffdcb3,
0xffffe2fe,
0xffffdbd1,
0xffffe2a0,
0xffffdae6,
0xffffe23f,
0xffffd9f0,
0xffffe1d7,
0xffffd8ee,
0xffffe168,
0xffffd7de,
0xffffe0f4,
0xffffd6c0,
0xffffe07a,
0xffffd592,
0xffffdff6,
0xffffd455,
0xffffdf6a,
0xffffd304,
0xffffded6,
0xffffd1a0,
0xffffde36,
0xffffd026,
0xffffdd8a,
0xffffce93,
0xffffdcd4,
0xffffcce6,
0xffffdc0e,
0xffffcb1c,
0xffffdb3a,
0xffffc92f,
0xffffda54,
0xffffc720,
0xffffd95b,
0xffffc4e8,
0xffffd84c,
0xffffc280,
0xffffd724,
0xffffbfe4,
0xffffd5e1,
0xffffbd0d,
0xffffd47c,
0xffffb9f3,
0xffffd2f3,
0xffffb68a,
0xffffd13f,
0xffffb2c6,
0xffffcf58,
0xffffae98,
0xffffcd36,
0xffffa9ee,
0xffffcacc,
0xffffa4ae,
0xffffc80e,
0xffff9ebe,
0xffffc4e8,
0xffff97f4,
0xffffc142,
0xffff9020,
0xffffbcfc,
0xffff86fc,
0xffffb7e8,
0xffff7c2c,
0xffffb1c7,
0xffff6f2e,
0xffffaa41,
0xffff5f47,
0xffffa0cc,
0xffff4b58,
0xffff948c,
0xffff319e,
0xffff8416,
0xffff0f22,
0xffff6cce,
0xfffede79,
0xffff4966,
0xfffe94a1,
0xffff0d21,
0xfffe1722,
0xfffe8fd6,
0xfffd1296,
0xfffcede6,
0xfff9ae4a,
0x00122986,
0x0025c680,
0x00023cec,
0x0004b3f4,
0x00012d50,
0x00027f80,
0x0000caa6,
0x0001b234,
0x000097b4,
0x000147fa,
0x000078a2,
0x00010708,
0x000063be,
0x0000db36,
0x000054c4,
0x0000bba4,
0x00004984,
0x0000a3ce,
0x000040c8,
0x0000912c,
0x000039d2,
0x00008232,
0x00003429,
0x000075e8,
0x00002f7c,
0x00006b9e,
0x00002b92,
0x000062e6,
0x00002842,
0x00005b66,
0x00002571,
0x000054e3,
0x00002307,
0x00004f2c,
0x000020f4,
0x00004a20,
0x00001f2a,
0x0000459f,
0x00001da0,
0x00004198,
0x00001c4d,
0x00003df6,
0x00001b2e,
0x00003aad,
0x00001a3a,
0x000037b0,
0x00001972,
0x000034f6,
0x000018d0,
0x00003276,
0x00001856,
0x0000302a,
0x000017fe,
0x00002e0d,
0x000017ce,
0x00002c1b,
0x000017c2,
0x00002a50,
0x000017df,
0x000028a8,
0x00001827,
0x00002722,
0x000018a0,
0x000025ba,
0x0000194e,
0x00002472,
0x00001a3c,
0x00002346,
0x00001b76,
0x0000223a,
0x00001d0c,
0x0000214c,
0x00001f18,
0x00002080,
0x000021bc,
0x00001fdd,
0x00002530,
0x00001f67,
0x000029c6,
0x00001f2e,
0x0000300e,
0x00001f48,
0x00003905,
0x00001fdd,
0x000046a2,
0x00002142,
0x00005d72,
0x00002439,
0x00008ae2,
0x00002afd,
0x00010f60,
0x00004050,
0x00154bbc,
0x0003a018,
0xfffecefc,
0xffffdea0,
0xffff6a40,
0xfffff7d3,
0xffff9bf2,
0xffffff6c,
0xffffb44e,
0x000002d1,
0xffffc2b4,
0x00000490,
0xffffcc28,
0x00000580,
0xffffd2d0,
0x000005fe,
0xffffd7bb,
0x00000636,
0xffffdb7c,
0x0000063e,
0xffffde6e,
0x00000627,
0xffffe0ca,
0x000005fb,
0xffffe2b5,
0x000005c0,
0xffffe44a,
0x00000578,
0xffffe59b,
0x00000528,
0xffffe6b5,
0x000004d1,
0xffffe7a3,
0x00000476,
0xffffe86c,
0x00000415,
0xffffe914,
0x000003b2,
0xffffe9a3,
0x0000034f,
0xffffea1a,
0x000002e7,
0xffffea7d,
0x0000027f,
0xffffeacd,
0x00000217,
0xffffeb0c,
0x000001ac,
0xffffeb3d,
0x00000142,
0xffffeb5e,
0x000000d6,
0xffffeb73,
0x0000006c
};



int32_t fft_512_input[1024] = {
0x002b6438,
0x00000000,
0x006ce3db,
0x00000000,
0x0076d960,
0x00000000,
0x003d62fc,
0x00000000,
0xffe4ff93,
0x00000000,
0xffa7cf46,
0x00000000,
0xffaab85b,
0x00000000,
0xffe29c95,
0x00000000,
0x001db01b,
0x00000000,
0x0029ceb8,
0x00000000,
0xfffadaab,
0x00000000,
0xffb42f4c,
0x00000000,
0xff8e0130,
0x00000000,
0xffaab853,
0x00000000,
0xfffc6a9f,
0x00000000,
0x004e805e,
0x00000000,
0x006c570d,
0x00000000,
0x0047e55a,
0x00000000,
0x0003630f,
0x00000000,
0xffd6c8d0,
0x00000000,
0xffe530ca,
0x00000000,
0x00223dde,
0x00000000,
0x005b9517,
0x00000000,
0x005f425f,
0x00000000,
0x00221226,
0x00000000,
0xffc8eaa3,
0x00000000,
0xff8e0138,
0x00000000,
0xff95fd0b,
0x00000000,
0xffd5332b,
0x00000000,
0x00190cb7,
0x00000000,
0x002e720e,
0x00000000,
0x0008440f,
0x00000000,
0xffc8eaa0,
0x00000000,
0xffa7cf45,
0x00000000,
0xffc6cd48,
0x00000000,
0x0017bb74,
0x00000000,
0x00661759,
0x00000000,
0x007da5cc,
0x00000000,
0x00510bb6,
0x00000000,
0x00036326,
0x00000000,
0xffcda293,
0x00000000,
0xffd3e207,
0x00000000,
0x000aa6c3,
0x00000000,
0x00404420,
0x00000000,
0x00432d60,
0x00000000,
0x00084427,
0x00000000,
0xffb42f73,
0x00000000,
0xff8097e8,
0x00000000,
0xff9159a4,
0x00000000,
0xffd9d664,
0x00000000,
0x002675f6,
0x00000000,
0x00432d51,
0x00000000,
0x00221232,
0x00000000,
0xffe4ffb7,
0x00000000,
0xffc32036,
0x00000000,
0xffde6444,
0x00000000,
0x00290a19,
0x00000000,
0x006f3d95,
0x00000000,
0x007da5d7,
0x00000000,
0x0047e58c,
0x00000000,
0xfff21488,
0x00000000,
0xffb60b95,
0x00000000,
0xffb89111,
0x00000000,
0xffee91a9,
0x00000000,
0x002675fd,
0x00000000,
0x002e721f,
0x00000000,
0xfffadae4,
0x00000000,
0xffaf8c2c,
0x00000000,
0xff853b3a,
0x00000000,
0xff9ec2e1,
0x00000000,
0xffee918a,
0x00000000,
0x004043f7,
0x00000000,
0x005f4259,
0x00000000,
0x003d6333,
0x00000000,
0xfffc96d6,
0x00000000,
0xffd46ef8,
0x00000000,
0xffe78a83,
0x00000000,
0x00290a0c,
0x00000000,
0x0066174b,
0x00000000,
0x006c572b,
0x00000000,
0x00304e9e,
0x00000000,
0xffd6c3b4,
0x00000000,
0xff99f696,
0x00000000,
0xff9ec2ee,
0x00000000,
0xffd9d64b,
0x00000000,
0x00190c96,
0x00000000,
0x0029cec7,
0x00000000,
0xffff7e42,
0x00000000,
0xffbcf589,
0x00000000,
0xff99f679,
0x00000000,
0xffb890e1,
0x00000000,
0x000aa676,
0x00000000,
0x005b94d6,
0x00000000,
0x0076d96a,
0x00000000,
0x004eb206,
0x00000000,
0x0005bd39,
0x00000000,
0xffd46f08,
0x00000000,
0xffde643b,
0x00000000,
0x0017bb47,
0x00000000,
0x004e803c,
0x00000000,
0x00510649,
0x00000000,
0x001439ae,
0x00000000,
0xffbcf5b2,
0x00000000,
0xff853b52,
0x00000000,
0xff915984,
0x00000000,
0xffd532d5,
0x00000000,
0x001dafd0,
0x00000000,
0x00373813,
0x00000000,
0x0014398f,
0x00000000,
0xffd6c3aa,
0x00000000,
0xffb60b80,
0x00000000,
0xffd3e1c0,
0x00000000,
0x00223d6e,
0x00000000,
0x006ce389,
0x00000000,
0x007fffc0,
0x00000000,
0x004eb228,
0x00000000,
0xfffc9715,
0x00000000,
0xffc32060,
0x00000000,
0xffc6cd2d,
0x00000000,
0xfffc6a4a,
0x00000000,
0x00326b2a,
0x00000000,
0x00373837,
0x00000000,
0xffff7e77,
0x00000000,
0xffaf8c68,
0x00000000,
0xff8097f5,
0x00000000,
0xff95fcbc,
0x00000000,
0xffe29c04,
0x00000000,
0x00326af9,
0x00000000,
0x00510623,
0x00000000,
0x00304ea5,
0x00000000,
0xfff214ac,
0x00000000,
0xffcda294,
0x00000000,
0xffe53076,
0x00000000,
0x002b63ab,
0x00000000,
0x006ce38b,
0x00000000,
0x0076d98f,
0x00000000,
0x003d6391,
0x00000000,
0xffe5002b,
0x00000000,
0xffa7cf7f,
0x00000000,
0xffaab81b,
0x00000000,
0xffe29c1b,
0x00000000,
0x001dafcf,
0x00000000,
0x0029cedb,
0x00000000,
0xfffadb26,
0x00000000,
0xffb42fbf,
0x00000000,
0xff8e013c,
0x00000000,
0xffaab7e3,
0x00000000,
0xfffc69f8,
0x00000000,
0x004e7fed,
0x00000000,
0x006c5717,
0x00000000,
0x0047e5ca,
0x00000000,
0x00036385,
0x00000000,
0xffd6c8ef,
0x00000000,
0xffe5307a,
0x00000000,
0x00223d61,
0x00000000,
0x005b94d4,
0x00000000,
0x005f4298,
0x00000000,
0x002212c0,
0x00000000,
0xffc8eb3b,
0x00000000,
0xff8e016b,
0x00000000,
0xff95fcbf,
0x00000000,
0xffd532a2,
0x00000000,
0x00190c5b,
0x00000000,
0x002e7221,
0x00000000,
0x0008447b,
0x00000000,
0xffc8eb07,
0x00000000,
0xffa7cf4b,
0x00000000,
0xffc6ccd7,
0x00000000,
0x0017bad1,
0x00000000,
0x006616f1,
0x00000000,
0x007da5e3,
0x00000000,
0x00510c36,
0x00000000,
0x000363ae,
0x00000000,
0xffcda2c3,
0x00000000,
0xffd3e1c5,
0x00000000,
0x000aa64f,
0x00000000,
0x004043e1,
0x00000000,
0x00432d98,
0x00000000,
0x000844ba,
0x00000000,
0xffb42fff,
0x00000000,
0xff80980c,
0x00000000,
0xff915948,
0x00000000,
0xffd9d5cb,
0x00000000,
0x0026758b,
0x00000000,
0x00432d59,
0x00000000,
0x00221298,
0x00000000,
0xffe5001d,
0x00000000,
0xffc3203f,
0x00000000,
0xffde63dc,
0x00000000,
0x00290984,
0x00000000,
0x006f3d3d,
0x00000000,
0x007da5ff,
0x00000000,
0x0047e61b,
0x00000000,
0xfff2151d,
0x00000000,
0xffb60bce,
0x00000000,
0xffb890d3,
0x00000000,
0xffee9134,
0x00000000,
0x002675b8,
0x00000000,
0x002e724b,
0x00000000,
0xfffadb67,
0x00000000,
0xffaf8ca7,
0x00000000,
0xff853b4d,
0x00000000,
0xff9ec276,
0x00000000,
0xffee90e5,
0x00000000,
0x00404385,
0x00000000,
0x005f425f,
0x00000000,
0x003d639c,
0x00000000,
0xfffc9745,
0x00000000,
0xffd46f0f,
0x00000000,
0xffe78a2b,
0x00000000,
0x00290987,
0x00000000,
0x00661703,
0x00000000,
0x006c5761,
0x00000000,
0x00304f36,
0x00000000,
0xffd6c44d,
0x00000000,
0xff99f6cd,
0x00000000,
0xff9ec2a9,
0x00000000,
0xffd9d5cb,
0x00000000,
0x00190c42,
0x00000000,
0x0029cee2,
0x00000000,
0xffff7eb5,
0x00000000,
0xffbcf5f5,
0x00000000,
0xff99f681,
0x00000000,
0xffb8906f,
0x00000000,
0x000aa5d0,
0x00000000,
0x005b9468,
0x00000000,
0x0076d979,
0x00000000,
0x004eb27d,
0x00000000,
0x0005bdb8,
0x00000000,
0xffd46f30,
0x00000000,
0xffde63f3,
0x00000000,
0x0017bacf,
0x00000000,
0x004e7ffd,
0x00000000,
0x00510682,
0x00000000,
0x00143a45,
0x00000000,
0xffbcf644,
0x00000000,
0xff853b7e,
0x00000000,
0xff915930,
0x00000000,
0xffd53244,
0x00000000,
0x001daf6b,
0x00000000,
0x0037381f,
0x00000000,
0x001439f6,
0x00000000,
0xffd6c40f,
0x00000000,
0xffb60b86,
0x00000000,
0xffd3e152,
0x00000000,
0x00223cd0,
0x00000000,
0x006ce329,
0x00000000,
0x007fffe0,
0x00000000,
0x004eb2af,
0x00000000,
0xfffc97a4,
0x00000000,
0xffc32096,
0x00000000,
0xffc6ccef,
0x00000000,
0xfffc69d6,
0x00000000,
0x00326aea,
0x00000000,
0x00373869,
0x00000000,
0xffff7f03,
0x00000000,
0xffaf8cec,
0x00000000,
0xff809810,
0x00000000,
0xff95fc57,
0x00000000,
0xffe29b63,
0x00000000,
0x00326a89,
0x00000000,
0x00510629,
0x00000000,
0x00304f0b,
0x00000000,
0xfff21515,
0x00000000,
0xffcda2a4,
0x00000000,
0xffe53015,
0x00000000,
0x002b631e,
0x00000000,
0x006ce33b,
0x00000000,
0x0076d9bf,
0x00000000,
0x003d6426,
0x00000000,
0xffe500c4,
0x00000000,
0xffa7cfb9,
0x00000000,
0xffaab7db,
0x00000000,
0xffe29ba1,
0x00000000,
0x001daf84,
0x00000000,
0x0029ceff,
0x00000000,
0xfffadba1,
0x00000000,
0xffb43032,
0x00000000,
0xff8e0148,
0x00000000,
0xffaab773,
0x00000000,
0xfffc6951,
0x00000000,
0x004e7f7c,
0x00000000,
0x006c5720,
0x00000000,
0x0047e639,
0x00000000,
0x000363fc,
0x00000000,
0xffd6c90e,
0x00000000,
0xffe5302a,
0x00000000,
0x00223ce4,
0x00000000,
0x005b9492,
0x00000000,
0x005f42d0,
0x00000000,
0x00221359,
0x00000000,
0xffc8ebd2,
0x00000000,
0xff8e019e,
0x00000000,
0xff95fc73,
0x00000000,
0xffd5321a,
0x00000000,
0x00190bfe,
0x00000000,
0x002e7234,
0x00000000,
0x000844e7,
0x00000000,
0xffc8eb6f,
0x00000000,
0xffa7cf50,
0x00000000,
0xffc6cc66,
0x00000000,
0x0017ba2e,
0x00000000,
0x00661689,
0x00000000,
0x007da5fa,
0x00000000,
0x00510cb5,
0x00000000,
0x00036436,
0x00000000,
0xffcda2f2,
0x00000000,
0xffd3e183,
0x00000000,
0x000aa5db,
0x00000000,
0x004043a3,
0x00000000,
0x00432dcf,
0x00000000,
0x0008454c,
0x00000000,
0xffb4308a,
0x00000000,
0xff80982f,
0x00000000,
0xff9158eb,
0x00000000,
0xffd9d531,
0x00000000,
0x00267520,
0x00000000,
0x00432d60,
0x00000000,
0x002212fd,
0x00000000,
0xffe50083,
0x00000000,
0xffc32049,
0x00000000,
0xffde6374,
0x00000000,
0x002908ee,
0x00000000,
0x006f3ce5,
0x00000000,
0x007da627,
0x00000000,
0x0047e6aa,
0x00000000,
0xfff215b2,
0x00000000,
0xffb60c06,
0x00000000,
0xffb89095,
0x00000000,
0xffee90bf,
0x00000000,
0x00267573,
0x00000000,
0x002e7277,
0x00000000,
0xfffadbeb,
0x00000000,
0xffaf8d22,
0x00000000,
0xff853b60,
0x00000000,
0xff9ec20b,
0x00000000,
0xffee9040,
0x00000000,
0x00404313,
0x00000000,
0x005f4265,
0x00000000,
0x003d6406,
0x00000000,
0xfffc97b5,
0x00000000,
0xffd46f26,
0x00000000,
0xffe789d3,
0x00000000,
0x00290902,
0x00000000,
0x006616bb,
0x00000000,
0x006c5796,
0x00000000,
0x00304fcf,
0x00000000,
0xffd6c4e7,
0x00000000,
0xff99f705,
0x00000000,
0xff9ec264,
0x00000000,
0xffd9d54a,
0x00000000,
0x00190bee,
0x00000000,
0x0029cefd,
0x00000000,
0xffff7f28,
0x00000000,
0xffbcf661,
0x00000000,
0xff99f688,
0x00000000,
0xffb88ffd,
0x00000000,
0x000aa529,
0x00000000,
0x005b93fa,
0x00000000,
0x0076d989,
0x00000000,
0x004eb2f4,
0x00000000,
0x0005be38,
0x00000000,
0xffd46f58,
0x00000000,
0xffde63ab,
0x00000000,
0x0017ba58,
0x00000000,
0x004e7fbe,
0x00000000,
0x005106bc,
0x00000000,
0x00143adc,
0x00000000,
0xffbcf6d7,
0x00000000,
0xff853ba9,
0x00000000,
0xff9158dc,
0x00000000,
0xffd531b2,
0x00000000,
0x001daf07,
0x00000000,
0x0037382b,
0x00000000,
0x00143a5e,
0x00000000,
0xffd6c475,
0x00000000,
0xffb60b8d,
0x00000000,
0xffd3e0e5,
0x00000000,
0x00223c33,
0x00000000,
0x006ce2c8,
0x00000000,
0x007fffff,
0x00000000,
0x004eb337,
0x00000000,
0xfffc9834,
0x00000000,
0xffc320cb,
0x00000000,
0xffc6ccb0,
0x00000000,
0xfffc6963,
0x00000000,
0x00326aa9,
0x00000000,
0x0037389c,
0x00000000,
0xffff7f8e,
0x00000000,
0xffaf8d6f,
0x00000000,
0xff80982b,
0x00000000,
0xff95fbf3,
0x00000000,
0xffe29ac3,
0x00000000,
0x00326a19,
0x00000000,
0x0051062e,
0x00000000,
0x00304f71,
0x00000000,
0xfff2157f,
0x00000000,
0xffcda2b3,
0x00000000,
0xffe52fb5,
0x00000000,
0x002b6290,
0x00000000,
0x006ce2ec,
0x00000000,
0x0076d9ee,
0x00000000,
0x003d64bb,
0x00000000,
0xffe5015c,
0x00000000,
0xffa7cff2,
0x00000000,
0xffaab79b,
0x00000000,
0xffe29b27,
0x00000000,
0x001daf38,
0x00000000,
0x0029cf23,
0x00000000,
0xfffadc1c,
0x00000000,
0xffb430a5,
0x00000000,
0xff8e0155,
0x00000000,
0xffaab704,
0x00000000,
0xfffc68aa,
0x00000000,
0x004e7f0a,
0x00000000,
0x006c572a,
0x00000000,
0x0047e6a8,
0x00000000,
0x00036473,
0x00000000,
0xffd6c92e,
0x00000000,
0xffe52fdb,
0x00000000,
0x00223c67,
0x00000000,
0x005b9450,
0x00000000,
0x005f4309,
0x00000000,
0x002213f2,
0x00000000,
0xffc8ec69,
0x00000000,
0xff8e01d0,
0x00000000,
0xff95fc28,
0x00000000,
0xffd53191,
0x00000000,
0x00190ba2,
0x00000000,
0x002e7247,
0x00000000,
0x00084553,
0x00000000,
0xffc8ebd6,
0x00000000,
0xffa7cf56,
0x00000000,
0xffc6cbf4,
0x00000000,
0x0017b98b,
0x00000000,
0x00661621,
0x00000000,
0x007da611,
0x00000000,
0x00510d34,
0x00000000,
0x000364bd,
0x00000000,
0xffcda322,
0x00000000,
0xffd3e141,
0x00000000,
0x000aa567,
0x00000000,
0x00404365,
0x00000000,
0x00432e06,
0x00000000,
0x000845de,
0x00000000,
0xffb43116,
0x00000000,
0xff809853,
0x00000000,
0xff91588f,
0x00000000,
0xffd9d497,
0x00000000,
0x002674b4,
0x00000000,
0x00432d68,
0x00000000,
0x00221362,
0x00000000,
0xffe500e9,
0x00000000,
0xffc32053,
0x00000000,
0xffde630c,
0x00000000,
0x00290858,
0x00000000,
0x006f3c8d,
0x00000000,
0x007da64e,
0x00000000,
0x0047e739,
0x00000000,
0xfff21647,
0x00000000,
0xffb60c3f,
0x00000000,
0xffb89057,
0x00000000,
0xffee9049,
0x00000000,
0x0026752e,
0x00000000,
0x002e72a3,
0x00000000,
0xfffadc6e,
0x00000000,
0xffaf8d9d,
0x00000000,
0xff853b73,
0x00000000,
0xff9ec1a0,
0x00000000,
0xffee8f9b,
0x00000000,
0x004042a1,
0x00000000,
0x005f426b,
0x00000000,
0x003d646f,
0x00000000,
0xfffc9824,
0x00000000,
0xffd46f3d,
0x00000000,
0xffe7897b,
0x00000000,
0x0029087d,
0x00000000,
0x00661672,
0x00000000,
0x006c57cb,
0x00000000,
0x00305067,
0x00000000,
0xffd6c580,
0x00000000,
0xff99f73c,
0x00000000,
0xff9ec21f,
0x00000000,
0xffd9d4c9,
0x00000000,
0x00190b9a,
0x00000000,
0x0029cf18,
0x00000000,
0xffff7f9b,
0x00000000,
0xffbcf6ce,
0x00000000,
0xff99f690,
0x00000000,
0xffb88f8b,
0x00000000,
0x000aa483,
0x00000000,
0x005b938c,
0x00000000,
0x0076d998,
0x00000000,
0x004eb36b,
0x00000000,
0x0005beb7,
0x00000000,
0xffd46f80,
0x00000000,
0xffde6362,
0x00000000,
0x0017b9e0,
0x00000000,
0x004e7f80,
0x00000000,
0x005106f5,
0x00000000,
0x00143b73,
0x00000000,
0xffbcf769,
0x00000000,
0xff853bd5,
0x00000000,
0xff915889,
0x00000000,
0xffd53121,
0x00000000,
0x001daea2,
0x00000000,
0x00373837,
0x00000000,
0x00143ac5,
0x00000000,
0xffd6c4da,
0x00000000,
0xffb60b93,
0x00000000,
0xffd3e077,
0x00000000,
0x00223b96,
0x00000000
};
