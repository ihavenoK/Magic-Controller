#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_CONV1D_KERNEL_0 {-4, 12, -60, 70, 49, 14, 47, -27, 44, 69, 40, -29, 8, 73, 16, 79, 12, 10, 22, -62, -73, 7, -17, -26, 22, 7, -45, 37, -31, -56, 11, 42, -89, -22, 19, -9, 7, -2, -31, 63, -39, 66, 78, 25, -33, 64, 1, -52, 17, 57, -50, -36, 52, -33, -6, 36, 17, -73, 56, -7, -48, -4, -80, 81, -39, -82, -24, -31, 25, 27, 5, -73, -52, -9, 47, 12, 9, 70, 34, 4, -1, -15, -36, 7, 71, 54, -38, 10, 48, 12, -25, 40, -70, -12, 38, 6, -7, 57, -3, -57, -46, -36, -6, 36, -5, -60, 11, -18, -78, 34, 34, -30, 30, -31, -32, -19, 41, 30, 10, 25, -20, 64, -28, -10, 80, -30, 73, -7, 17, -50, 46, 4, 52, 55, 25, -57, -18, 11, -62, -11, -45, -52, -18, -79, -7, -4, -8, -41, 84, 8, -40, 52, -67, 85, -58, -62, 37, -77, -26, 16, -44, 20, -38, -19, 54, -34, 24, 52, -57, -39, 29, 52, -20, 56, 26, -50, -29, 29, -76, 30, -9, -9, -50, -31, -14, 20, -26, 40, -50, 72, -41, -82, -21, 14, -30, 55, 36, -1, -72, 12, -17, -70, 84, 55, -89, 25, -60, -38, -62, 50, -33, 38, 73, -75, 13, 7, -5, -20, 64, -32, -25, -13, -44, -37, 51, -54, 58, -59, -56, 9, 1, 13, 73, -34, -12, 12, 8, -68, 2, -62, -33, 65, -53, -61, 12, 56, 7, 72, -28, 34, 74, -41, 62, 13, 14, 60, 17, -35, 65, -22, 16, 15, 24, 69, 65, -21, -14, 8, -7, -6}

#define TENSOR_CONV1D_KERNEL_0_DEC_BITS {8}

#define TENSOR_CONV1D_BIAS_0 {7, -14, 51, 70, 12, 49, 90, 28, 37, -12, 76, 70, 23, 46, 27, 52, 91, 48, 38, 60, 93, 58, 87, 15, 37, 46, 86, 13, 17, 45}

#define TENSOR_CONV1D_BIAS_0_DEC_BITS {9}

#define CONV1D_BIAS_LSHIFT {4}

#define CONV1D_OUTPUT_RSHIFT {8}

#define TENSOR_CONV1D_1_KERNEL_0 {-63, -4, 1, 17, 7, -38, -8, -16, -22, 32, -64, 48, -3, 20, 34, 54, -9, -21, 51, -6, -4, -32, -32, 25, -10, 10, -8, 12, -34, 35, 25, -29, -38, 15, -18, -3, 34, 52, -15, -27, -52, 64, 31, 3, 44, 49, 9, -9, 46, -49, 10, 37, 61, 3, 15, 42, -18, 0, -48, 10, -37, 24, 28, 45, -71, -57, 59, 37, 10, -1, -17, 40, 2, -52, -50, -19, -25, 25, 20, -44, 52, 33, 66, 27, -33, -48, -39, -5, -36, -9, -3, 31, -12, -16, 19, 45, 81, -13, 35, 49, 53, -4, -3, 18, -31, -35, -14, 17, -5, -6, -18, 39, -13, -17, 36, 7, 24, -28, -27, 7, -35, -40, -31, -21, -6, -3, 33, -14, -19, 8, -36, 46, -8, 49, 7, -1, 27, 8, -31, 72, 49, 34, -60, -41, -12, -18, -6, 41, 66, -34, 73, 23, 24, -26, 57, -11, -25, 20, -37, -37, 4, -32, 42, -19, 7, -85, -20, 43, 13, 53, 11, 19, 31, 7, -27, -32, -77, -51, 71, 13, -12, -21, 40, 14, -25, -8, -2, 19, 72, -41, -20, 27, -21, -60, 50, 38, -68, 33, 37, 41, 60, 0, 13, 38, 35, -17, -17, -19, -10, 56, -24, -33, 35, 18, -6, -39, 6, 44, -21, 36, -60, 10, -29, -73, -11, 6, 11, 63, 53, -20, 16, 30, -47, -8, 53, 29, 34, 24, 50, 12, -30, -32, 29, 75, 59, 56, -18, 17, 58, -56, 17, 3, -7, 2, -34, 36, -51, 78, 10, 51, -6, 50, -57, 57, 18, -15, -37, 19, -2, 30, 46, 41, -11, 60, -8, 21, -55, 36, 29, -43, -45, 6, 48, 36, 17, 41, -46, 0, 46, 24, -13, 36, -29, 52, 42, 42, 19, 1, 70, -10, 24, 33, 77, -21, -29, 62, 8, 86, 51, 17, -60, 18, 48, -11, 54, 42, -63, 68, -5, -6, -22, 60, -9, -2, -42, 9, -25, 0, -35, 14, 29, 30, 45, 53, 49, 45, 18, 62, 39, 46, -73, -28, 25, -22, 23, -16, -68, 81, 27, 70, 22, 40, -43, -5, 44, -16, -29, -65, 10, 72, 1, -9, 47, 14, -3, 36, 12, -40, 52, -38, 4, -17, -11, -38, 24, -27, -39, 21, -33, -36, 27, 40, -40, -42, -10, 39, 7, -3, 31, 1, -22, -33, -38, -45, -49, -36, 49, -24, 32, -25, -44, 37, 48, 27, -29, -29, 32, -24, -49, -35, -4, 3, 15, -50, -55, 28, 15, 26, 2, -54, -18, -47, -38, 34, 24, -51, 5, 51, -36, 19, -7, -16, -48, -17, -39, -54, -50, 40, 39, -50, -1, -41, -15, -17, 51, 37, 28, -18, 5, -32, -16, -67, -20, -43, 17, -46, 67, -47, 39, -54, 92, 40, 12, 51, -60, -16, 62, 20, -52, -39, 13, 3, 70, -7, -6, 52, 91, 65, 18, 30, -1, 1, 28, -5, -17, 10, 84, -64, -40, 26, 63, 6, 32, 64, 47, 65, 88, 32, -31, 13, 36, -16, 16, -50, -36, 31, 21, -29, -31, -12, 45, -47, -32, 52, 33, 34, 29, 19, 11, 38, 79, 89, 30, 62, 8, 13, 83, -50, -51, -22, 39, 3, 86, 1, -51, 74, 42, 29, -1, 25, -49, -13, 83, 47, -22, 78, 70, -15, 13, 21, 75, 4, -85, 19, 38, -8, -2, 30, -80, 41, 22, 28, -29, 2, 22, 13, 70, -56, -24, 46, 28, -36, 38, 4, 2, -11, 54, 34, -64, -67, 30, 84, -44, 21, 5, 31, 72, -39, -14, 15, 64, 23, 68, -38, -42, 66, 60, 0, -11, -35, -50, 13, 1, 77, -60, 60, 51, 14, -79, 14, 44, 5, -9, 0, -30, 91, 99, -22, -11, 44, 108, 54, 18, 14, -35, 62, 75, 48, -24, -34, -49, -28, -16, -33, -51, -36, 45, 28, -47, -33, -25, 48, -19, -29, -12, -47, 38, -17, 29, -37, 32, 4, -6, -14, -38, 44, -46, -39, 48, -45, -37, 27, -51, 19, -24, 37, -39, 42, -28, 7, -10, 27, -11, -56, 48, 44, -28, 1, -56, 4, -3, -39, -3, -31, 22, -53, 42, -25, -26, -6, -2, 16, 1, -51, -36, 13, -47, -18, -39, 35, 19, -7, -13, 32, 3, 2, -31, -47, 5, -31, 5, -36, 31, 48, 18, -51, 31, -15, -20, -29, 48, -25, -18, -11, 0, 32, -27, 38, -33, 8, 24, -6, 8, -23, -32, 6, -38, -6, 45, -11, -6, 23, 38, -53, 44, -19, 48, -62, -15, -24, -53, 2, -35, 4, 8, 38, 22, 44, 36, -23, -57, -16, 30, 37, -17, 30, 21, 42, -38, 27, -40, -45, -52, -21, 30, 41, -28, -47, -32, -26, 38, -15, 57, -23, -23, 21, 23, 22, -22, -1, 11, -17, -57, -39, -13, -45, -28, 18, -26, 51, 13, 53, 41, 37, -13, 27, -42, -8, -27, 35, 35, 48, -31, 5, 49, 64, 35, -69, 37, -6, -10, 21, 27, -3, 19, 36, 97, -74, -14, -18, 55, -39, 76, -31, 7, 44, 88, 13, -18, -41, 41, 53, 26, -28, -6, 67, 48, -19, -6, -15, 28, -18, 59, 82, 10, 52, 26, -73, -16, 6, -24, 58, 18, 49, -54, 52, 18, 82, 8, 14, -46, -1, 3, 24, 46, -20, 76, -24, -34, 39, 32, 18, -36, 49, -26, 6, 24, 17, -12, -55, -3, -24, 55, -27, -57, 4, 14, 62, 12, 34, -5, -26, 34, 66, -19, -37, -74, -9, -21, 17, -13, 40, -41, 22, 56, -42, -24, -32, 46, 27, 59, -47, 8, -58, -47, 36, -34, -9, -15, 57, 2, 21, 80, 4, 54, -17, -53, 13, 52, 51, -38, -34, -23, 36, 9, 16, -63, -2, 25, -4, 32, 25, 6, -20, -56, 34, 4, 65, 61, -3, 58, -8, 60, 77, 28, -31, 0, 65, 26, -3, 38, 1, -42, 45, 47, 32, -61, 9, -59, -42, 9, 17, -41, 12, -68, -46, 21, -17, 45, 14, -61, -10, -15, -32, -51, -16, 37, -50, 43, -43, 31, 86, 25, 14, -24, 40, 19, 15, -12, 17, 23, -13, 86, 67, -9, 14, 15, -3, -18, -53, -48, -26, -4, 46, 11, 4, 1, -25, 15, -24, 28, -7, -18, 15, 34, 9, 51, -27, 59, -25, 50, 2, 67, -34, 63, 44, 94, -17, -11, -37, -8, -30, -23, 6, -7, -27, 90, -63, 61, 5, 61, 50, 39, 58, 50, -25, 28, 29, 50, -2, 36, 7, -18, 17, 47, 3, 49, 41, 23, -35, 22, 2, -48, -17, -23, 31, -1, 31, -42, -20, 52, 45, 7, -21, -12, -19, 67, -35, 26, -51, 45, -8, 47, -12, -35, -12, 17, -4, -20, -11, 31, 13, 7, 71, -54, 32, 75, 7, 20, 6, 23, 1, 28, 46, 32, 51, 75, -60, 2, -21, -4, -10, 8, -68, 39, 68, 20, -26, 36, -4, -11, -14, -35, -13, 44, 8, 58, -48, -7, 18, 10, 34, -53, 37, -9, -24, 71, 15, 18, -40, 35, 49, 26, -39, -65, -11, 5, 40, 38, 38, 29, -34, -5, 50, -6, 8, -23, 46, 55, -11, 28, -29, 76, 11, -31, -62, 56, -49, -24, -55, -13, -20, 11, 30, -24, 18, 37, 84, -28, 23, 21, 57, -11, 42, -52, 7, -29, -44, -23, 24, 35, -75, -11, 67, 62, -69, -17, -64, 21, -39, -29, 47, 1, 18, -39, 71, -41, 18, 10, -26, -36, 63, -33, -62, 45, 8, 7, -36, -32, -14, 67, -13, -29, 72, 67, 31, 36, -27, -32, -76, -2, 27, 39, 8, -14, -7, 48, 40, 47, -11, 63, 9, 14, -43, -19, -34, 15, -25, -18, 55, -46, 11, -39, -40, -23, 39, -14, -49, -33, 74, -31, 44, -19, -33, -52, 66, 10, -45, 42, 64, -20, 61, -1, -6, 6, 38, -23, 27, 2, -23, -66, 15, 37, 11, 49, 16, 28, 5, 46, -25, -14, 40, -48, 27, 46, 17, 31, 38, 30, 37, 31, 26, -34, -41, 26, -34, -1, -46, -21, -51, -47, 14, -31, -21, 54, -5, -53, -35, 13, 43, -34, -47, -13, -28, -34, 27, 9, 36, -49, 55}

#define TENSOR_CONV1D_1_KERNEL_0_DEC_BITS {8}

#define TENSOR_CONV1D_1_BIAS_0 {35, 71, 58, 80, -9, 69, 104, -19, -31, 25, 18, 114, -10, -43, 40}

#define TENSOR_CONV1D_1_BIAS_0_DEC_BITS {10}

#define CONV1D_1_BIAS_LSHIFT {3}

#define CONV1D_1_OUTPUT_RSHIFT {9}

#define TENSOR_DENSE_KERNEL_0 {66, 16, 72, 18, 28, 7, -62, 12, 74, 56, 37, -34, -48, 44, -77, -24, 41, -65, 40, -37, -58, -9, -89, 38, -59, 0, -23, -51, 48, 17, -41, -8, 55, -56, 29, 37, -58, -1, 28, 18, -5, -78, -29, -46, 45, 11, 43, 4, -65, -25, -33, -49, 20, -5, -72, -13, -68, 30, 82, 59, 53, -7, 23, -6, -6, -10, -45, 73, -6, -64, 27, -18, -28, 89, 56, 1, -67, -9, -66, -19, -54, -63, -20, 10, -25, 24, 9, -51, -71, -6, 12, -40, -75, -53, 29, 38, -34, -92, 58, 36, 22, -79, -55, -14, -56, 11, -49, 10, 64, -26, 52, 12, 28, -39, -17, 20, 45, 31, -3, -45, 1, -27, 52, -32, -18, 14, -37, 74, 27, -2, -11, 35, 25, 60, -63, -54, 46, 4, 73, -33, 11, -26, 31, -7, -37, -80, -20, 26, -107, 6, 44, 8, 54, -13, 38, -76, -10, 59, 11, -12, -30, -26, 62, -103, -60, 11, -36, 24, 40, -9, 24, -12, 58, -10, 19, -19, 42, 18, 43, 23, 27, 69, 4, 18, 49, -63, 82, 39, 60, 21, 55, 21, 23, 67, -79, -9, -69, 19, -48, -46, 38, 24, -61, 20, 39, -27, -6, -103, -44, -44, -63, -41, 47, -5, -77, 14, 50, 29, -44, 31, -48, 24, 50, 61, -7, 8, -33, 28, 17, -99, 19, 56, -19, -65, -36, 83, 20, 25, 29, 53, -11, 6, -21, -22, -97, -17, -1, -45, -34, -52, 63, 42, 6, -87, -2, -47, 51, -24, -12, -57, -51, -8, -65, 22, -68, -6, 10, -42, -53, 61, -58, 50, 43, -22, -40, 59, -72, 27, 69, -97, -96, -12, -28, 41, -8, 25, -72, 89, -22, -85, 13, -15, -40, -66, -20, 41, -26, 17, -55, 3, -39, -28, -6, 2, 43, -61, -90, 15, 23, 44, 9, -37, 30, -57, -40, -77, 36, -32, 52, -37, 0, -24, -86, 20, -2, 30, -7, 53, -38, 13, 51, 25, -27, -58, 5, -25, -25, 45, -12, 29, 1, -13, 32, 28, -67, -64, -3, -38, -8, -49, 28, 38, -26, 20, -68, -20, -7, -46, -83, 40, 20, 41, -27, 49, 40, -42, 51, 19, -16, 12, 46, 14, -71, 68, 16, -55, 27, -91, -60, -29, -36, 33, 52, -52, -54, -40, -2, -64, -46, 105, -86, -21, -63, -37, 53, -6, -9, 4, -60, 36, -9, -73, -6, -39, -1, 37, -1, -18, -27, 38, 52, -10, 42, -84, -47, 5, 24, -20, 34, 33, -46, -12, 22, -47, -47, -10, 23, -69, -13, -84, -62, -47, -86, -34, 8, 2, 44, -42, 13, 27, 21, -64, -25, -47, 6, 58, 56, 23, -62, -86, -38, 3, 3, 32, -17, 4, 40, 53, 30, 12, -23, 18, 15, -23, 44, -68, -42, 47, 33, 28, 4, 76, -41, -86, -14, -28, 59, -70, -21, -45, -86, -2, -1, -86, 0, -38, -93, 35, -54, -31, 33, 30, -51, 29, 87, -21, -79, -9, 42, 25, -54, -40, 45, 13, -31, -38, 27, -2, -38, 17, -2, 8, 23, 16, -51, -4, -69, -44, -33, 56, -52, -68, -16, -76, 32, -25, -59, 41, -87, -75, -37, -82, 90, 3, -17, 13, 17, 33, -45, -20, -26, -71, 22, -60, 44, -53, 13, -24, 28, 67, -21, -54, -25, -18, -71, -11, -30, -60, -28, 1, 18, 6, -2, -64, 26, -42, -21, 25, -32, 31, -54, 41, -87, 40, -26, -22, -67, -60, -63, -39, -28, 70, -50, -58, 1, 22, -31, 10, -7, 15, -48, -4, 67, 52, -63, 24, 21, -24, 16, -65, -8, -11, -62, -7, -45, 48, -3, 4, -20, -14, -15, -9, -67, -94, 14, -73, 9, -26, 28, -62, -39, 18, -75, 10, -78, -50, -5, 35, -7, 29, -7, 59, 22, -1, -23, -6, 37, -62, -14, 16, -4, -53, -49, 5, -57, -5, -36, -92, -69, -76, 45, -40, -9, 40, 19, 0, 33, -61, -56, -46, -11, 8, -66, -27, -50, 22, -7, -52, 38, 15, 16, -33, -31, -8, -38, -11, 2, -47, -4, -18, -14, -37, -43, -33, -16, -7, -26, 12, -79, -49, -4, -62, -44, -97, -34, 56, -60, -18, -63, -26, -73, -19, -49, -33, -42, -6, -18, 34, 3, -37, 38, -4, -9, -55, 11, -77, -12, 19, 43, -6, -67, -39, 16, -49, -58, 19, -19, 0, -89, -67, -7, 47, -68, 17, -55, 57, -2, 43, 33, -51, -8, -41, -34, -82, -50, -69, -50, 60, -73, 7, 20, 49, 59, -9, 23, 8, -29, -1, -20, -77, -40, 27, -1, 53, -56, -30, -56, -30, -40, 16, -4, -39, -37, 38, 19, 38, 35, 8, -40, 0, 20, -61, -9, 24, 21, 47, -24, 26, -20, -30, -29, -4, -34, 42, -62, -73, -39, 13, 39, -65, -2, 55, 13, -89, 8, -24, -67, -49, -51, 43, -23, 6, -16, 14, -72, 21, -30, 41, -22, -67, -50, 54, -47, -52, 3, -45, 17, -26, -47, -22, 2, 6, -24, 39, -33, -28, -69, -60, -48, -18, 18, 19, -76, -17, 20, -70, -77, -64, -40, -6, -2, -84, 11, 14, -64, -38, -29, -53, -77, -51, -20, -69, -65, -43, -3, -16, -39, 26, -10, 25, -35, 38, -47, 23, -14, -19, 61, -6, -17, -21, -3, 9, -22, -55, -92, -96, -27, -61, -24, -63, 16, 18, -29, -7, 38, 4, 9, -84, 26, 43, 3, -68, -53, -16, -61, -60, 15, -42, -32, -20, 33, -49, 47, -72, -11, 16, -89, -11, 33, 30, 44, -90, 61, -6, -17, 15, 10, -8, -63, -63, 26, -85, 9, -31, -32, 43, -25, 33, -2, -52, 36, -18, 33, -5, -13, 14, -39, -58, -4, 8, -79, -36, -15, 40, -50, 36, -59, 16, -50, -72, -26, 13, -93, -25, 59, -24, -14, -36, -20, -42, -29, -9, -58, -41, 30}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {8}

#define TENSOR_DENSE_BIAS_0 {46, 35, -28, -63, 24, 39, -65, -54, -65, -64, -56, -73, -57}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {10}

#define DENSE_BIAS_LSHIFT {2}

#define DENSE_OUTPUT_RSHIFT {8}


/* output q format for each layer */
#define INPUT_1_OUTPUT_DEC 5
#define INPUT_1_OUTPUT_OFFSET 0
#define CONV1D_OUTPUT_DEC 5
#define CONV1D_OUTPUT_OFFSET 0
#define RE_LU_OUTPUT_DEC 5
#define RE_LU_OUTPUT_OFFSET 0
#define CONV1D_1_OUTPUT_DEC 4
#define CONV1D_1_OUTPUT_OFFSET 0
#define RE_LU_1_OUTPUT_DEC 4
#define RE_LU_1_OUTPUT_OFFSET 0
#define MAX_POOLING1D_OUTPUT_DEC 4
#define MAX_POOLING1D_OUTPUT_OFFSET 0
#define FLATTEN_OUTPUT_DEC 4
#define FLATTEN_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 4
#define DENSE_OUTPUT_OFFSET 0
#define DROPOUT_OUTPUT_DEC 4
#define DROPOUT_OUTPUT_OFFSET 0
#define SOFTMAX_OUTPUT_DEC 7
#define SOFTMAX_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[450] = {0};

const nnom_shape_data_t tensor_input_1_dim[] = {150, 3};
const nnom_qformat_param_t tensor_input_1_dec[] = {5};
const nnom_qformat_param_t tensor_input_1_offset[] = {0};
const nnom_tensor_t tensor_input_1 = {
    .p_data = (void*)nnom_input_data,
    .dim = (nnom_shape_data_t*)tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};

const nnom_io_config_t input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t*)&tensor_input_1
};
const int8_t tensor_conv1d_kernel_0_data[] = TENSOR_CONV1D_KERNEL_0;

const nnom_shape_data_t tensor_conv1d_kernel_0_dim[] = {3, 3, 30};
const nnom_qformat_param_t tensor_conv1d_kernel_0_dec[] = TENSOR_CONV1D_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_kernel_0 = {
    .p_data = (void*)tensor_conv1d_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};
const int8_t tensor_conv1d_bias_0_data[] = TENSOR_CONV1D_BIAS_0;

const nnom_shape_data_t tensor_conv1d_bias_0_dim[] = {30};
const nnom_qformat_param_t tensor_conv1d_bias_0_dec[] = TENSOR_CONV1D_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_bias_0 = {
    .p_data = (void*)tensor_conv1d_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv1d_output_shift[] = CONV1D_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv1d_bias_shift[] = CONV1D_BIAS_LSHIFT;
const nnom_conv2d_config_t conv1d_config = {
    .super = {.name = "conv1d"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv1d_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv1d_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv1d_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv1d_bias_shift, 
    .filter_size = 30,
    .kernel_size = {3},
    .stride_size = {3},
    .padding_size = {0, 0},
    .dilation_size = {1},
    .padding_type = PADDING_VALID
};
const int8_t tensor_conv1d_1_kernel_0_data[] = TENSOR_CONV1D_1_KERNEL_0;

const nnom_shape_data_t tensor_conv1d_1_kernel_0_dim[] = {3, 30, 15};
const nnom_qformat_param_t tensor_conv1d_1_kernel_0_dec[] = TENSOR_CONV1D_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_1_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_1_kernel_0 = {
    .p_data = (void*)tensor_conv1d_1_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};
const int8_t tensor_conv1d_1_bias_0_data[] = TENSOR_CONV1D_1_BIAS_0;

const nnom_shape_data_t tensor_conv1d_1_bias_0_dim[] = {15};
const nnom_qformat_param_t tensor_conv1d_1_bias_0_dec[] = TENSOR_CONV1D_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_1_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_1_bias_0 = {
    .p_data = (void*)tensor_conv1d_1_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv1d_1_output_shift[] = CONV1D_1_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv1d_1_bias_shift[] = CONV1D_1_BIAS_LSHIFT;
const nnom_conv2d_config_t conv1d_1_config = {
    .super = {.name = "conv1d_1"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv1d_1_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv1d_1_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv1d_1_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv1d_1_bias_shift, 
    .filter_size = 15,
    .kernel_size = {3},
    .stride_size = {3},
    .padding_size = {0, 0},
    .dilation_size = {1},
    .padding_type = PADDING_VALID
};

const nnom_pool_config_t max_pooling1d_config = {
    .super = {.name = "max_pooling1d"},
    .padding_type = PADDING_VALID,
    .output_shift = 0,
    .kernel_size = {3},
    .stride_size = {3},
    .num_dim = 1
};

const nnom_flatten_config_t flatten_config = {
    .super = {.name = "flatten"}
};
const int8_t tensor_dense_kernel_0_data[] = TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {75, 13};
const nnom_qformat_param_t tensor_dense_kernel_0_dec[] = TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_kernel_0 = {
    .p_data = (void*)tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_bias_0_data[] = TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t tensor_dense_bias_0_dim[] = {13};
const nnom_qformat_param_t tensor_dense_bias_0_dec[] = TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_bias_0 = {
    .p_data = (void*)tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_output_shift[] = DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_bias_shift[] = DENSE_BIAS_LSHIFT;
const nnom_dense_config_t dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_bias_shift
};

const nnom_softmax_config_t softmax_config = {
    .super = {.name = "softmax"}
};
static int8_t nnom_output_data[13] = {0};

const nnom_shape_data_t tensor_output0_dim[] = {13};
const nnom_qformat_param_t tensor_output0_dec[] = {SOFTMAX_OUTPUT_DEC};
const nnom_qformat_param_t tensor_output0_offset[] = {0};
const nnom_tensor_t tensor_output0 = {
    .p_data = (void*)nnom_output_data,
    .dim = (nnom_shape_data_t*)tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t*)&tensor_output0
};
/* model version */
#define NNOM_MODEL_VERSION (10000*0 + 100*4 + 3)

/* nnom model */
static nnom_model_t* nnom_model_create(void)
{
	static nnom_model_t model;
	nnom_layer_t* layer[10];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&input_1_config);
	layer[1] = model.hook(conv2d_s(&conv1d_config), layer[0]);
	layer[2] = model.active(act_relu(), layer[1]);
	layer[3] = model.hook(conv2d_s(&conv1d_1_config), layer[2]);
	layer[4] = model.active(act_relu(), layer[3]);
	layer[5] = model.hook(maxpool_s(&max_pooling1d_config), layer[4]);
	layer[6] = model.hook(flatten_s(&flatten_config), layer[5]);
	layer[7] = model.hook(dense_s(&dense_config), layer[6]);
	layer[8] = model.hook(softmax_s(&softmax_config), layer[7]);
	layer[9] = model.hook(output_s(&output0_config), layer[8]);
	model_compile(&model, layer[0], layer[9]);
	return &model;
}
