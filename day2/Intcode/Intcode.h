#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef uint64_t INTCODE;

typedef enum
{
    INTCODE_OPCODE_ADD = 1u,
    INTCODE_OPCODE_MULTIPLY = 2u,
    INTCODE_OPCODE_HALT = 99u,
} INTCODE_OPCODE;

typedef enum
{
    INTCODE_OFFSET_INSTR,
    INTCODE_OFFSET_OP1,
    INTCODE_OFFSET_OP2,
    INTCODE_OFFSET_DST,
} INTCODE_OFFSET;

typedef enum
{
    INTCODE_OPCODE_SIZE_ADD = 4u,
    INTCODE_OPCODE_SIZE_MULTIPLY = 4u,
    INTCODE_OPCODE_SIZE_HALT = 1u,
} INTCODE_OPCODE_SIZE;

typedef enum
{
    INTCODE_STATUS_OPCODE_UNKNOWN = -5,
    INTCODE_STATUS_SEGFAULT = -4,
    INTCODE_STATUS_FORMAT_ERROR = -3,
    INTCODE_STATUS_MULTIPLY_ERROR = -2,
    INTCODE_STATUS_ADD_ERROR = -1,
    INTCODE_STATUS_RUNNING = 0,
    INTCODE_STATUS_HALT = 99,
} INTCODE_STATUS;

int32_t IntcodeInit(INTCODE **input, size_t *input_size);
int32_t IntcodeReset(void);
int32_t IntcodeExit(void);
int32_t IntcodeExec(INTCODE *input, size_t input_size);
int32_t IntcodeReverse(INTCODE *input, size_t input_size, INTCODE target);
void IntcodePrintArray(INTCODE const *array, size_t size);