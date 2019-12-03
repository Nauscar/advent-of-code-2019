#include <Intcode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

#define NUMBER_BASE (10)

INTCODE *intcodeSP = NULL;
INTCODE *intcodeBP = NULL;
INTCODE *intcodeOOB = NULL;
INTCODE *intcodeBackup = NULL;
static size_t backupSize = 0;

static inline INTCODE_STATUS intcodeAdd(void)
{
    assert(intcodeSP[INTCODE_OFFSET_INSTR] == INTCODE_OPCODE_ADD);

    // Get the operand addresses and check for out of bounds memory access.
    INTCODE const * const op1 = intcodeBP + intcodeSP[INTCODE_OFFSET_OP1];
    INTCODE const * const op2 = intcodeBP + intcodeSP[INTCODE_OFFSET_OP2];
    INTCODE * const dst = intcodeBP + intcodeSP[INTCODE_OFFSET_DST];
    if ((op1 >= intcodeOOB) || (op2 >= intcodeOOB) || (dst >= intcodeOOB))
    {
        return INTCODE_STATUS_SEGFAULT;
    }

    // Perform the add operation and store at destination.
    *dst = (*op1) + (*op2);

    // Increment the SP by the size of the ADD instruction.
    intcodeSP += INTCODE_OPCODE_SIZE_ADD;
    return INTCODE_STATUS_RUNNING;
}

static inline INTCODE_STATUS intcodeMultipy(void)
{
    assert(intcodeSP[INTCODE_OFFSET_INSTR] == INTCODE_OPCODE_MULTIPLY);

    // Get the operand addresses and check for out of bounds memory access.
    INTCODE const * const op1 = intcodeBP + intcodeSP[INTCODE_OFFSET_OP1];
    INTCODE const * const op2 = intcodeBP + intcodeSP[INTCODE_OFFSET_OP2];
    INTCODE * const dst = intcodeBP + intcodeSP[INTCODE_OFFSET_DST];
    if ((op1 >= intcodeOOB) || (op2 >= intcodeOOB) || (dst >= intcodeOOB))
    {
        return INTCODE_STATUS_SEGFAULT;
    }

    // Perform the multiply operation and store at destination.
    *dst = (*op1) * (*op2);

    // Increment the SP by the size of the MULTIPLY instruction.
    intcodeSP += INTCODE_OPCODE_SIZE_MULTIPLY;
    return INTCODE_STATUS_RUNNING;
}

static inline INTCODE_STATUS intcodeHalt(void)
{
    assert(intcodeSP[INTCODE_OFFSET_INSTR] == INTCODE_OPCODE_HALT);

    // Increment the SP by the size of the HALT instruction.
    // NOTE: the return of INTCODE_STATUS_HALT will end the consume loop.
    intcodeSP += INTCODE_OPCODE_SIZE_HALT;
    return INTCODE_STATUS_HALT;
}

static inline INTCODE_STATUS intcodeConsumeOpcode(void)
{
    // Check for an uninitialized stack.
    // Since this can only be called locally, this should never occur.
    if (unlikely(intcodeSP == NULL) || intcodeBP == NULL || intcodeOOB == NULL)
    {
        return INTCODE_STATUS_SEGFAULT;
    }

    // Get opcode from the SP.
    INTCODE_STATUS status = INTCODE_STATUS_RUNNING;
    INTCODE_OPCODE opcode = (INTCODE_OPCODE)intcodeSP[INTCODE_OFFSET_INSTR];
    switch(opcode)
    {
        case INTCODE_OPCODE_ADD:
            status = intcodeAdd();
            break;
        case INTCODE_OPCODE_MULTIPLY:
            status = intcodeMultipy();
            break;
        case INTCODE_OPCODE_HALT:
            status = intcodeHalt();
            break;
        default:
            status = INTCODE_STATUS_OPCODE_UNKNOWN;
            break;
    }

    if (status != INTCODE_STATUS_HALT && status != INTCODE_STATUS_RUNNING)
        printf("Opcode failure status: %i\n", status);
    return status;
}

int32_t IntcodeInit(INTCODE **input, size_t *input_size)
{
    if (input == NULL)
    {
        return EXIT_FAILURE;
    }

    *input_size = 0;
    
    FILE *fp = fopen("./input.in", "r");
    if (fp == NULL)
    {
        puts("Failed to open input file.");
        return EXIT_FAILURE;
    }

    size_t count = 0;
    char c = '\0';
    while (fscanf(fp," %c", &c) == 1)
    {
        if (c == ',')
        {
            ++count;
        }
    }
    if (c != ',')
    {
        ++count;
    }
    rewind(fp);

    int32_t status = EXIT_FAILURE;
    *input_size = count * sizeof(INTCODE);
    INTCODE *newInput = realloc(*input, *input_size);
    backupSize = *input_size;
    INTCODE *newBackup = realloc(intcodeBackup, backupSize);
    if (newInput == NULL || newBackup == NULL)
    {
        puts("realloc failure.");
        goto fail;
    }
    *input = newInput;
    intcodeBackup = newBackup;

    static char number[20] = {0};
    uint8_t i = 0;
    size_t index = 0;
    while ((i < sizeof(number)) && (fscanf(fp," %c", &number[i]) == 1) && (index < count))
    {
        if (number[i] == ',')
        {
            number[i] = '\0';
            char *endPtr = NULL;
            (*input)[index++] = strtoul(number, &endPtr, NUMBER_BASE);
            if (endPtr == NULL)
            {
                // Failed conversion.
                puts("Conversion failed.");
                status = EXIT_FAILURE;
                goto fail;
            }
            i = 0;
            memset(number, 0, sizeof(number));
            continue;
        }
        ++i;
    }
    memcpy(intcodeBackup, *input, backupSize);

fail:
    fclose(fp);
    return status;
}

int32_t IntcodeReset(void)
{
    if (intcodeBackup == NULL || intcodeBP == NULL)
    {
        return EXIT_FAILURE;
    }
    memcpy(intcodeBP, intcodeBackup, backupSize);
    intcodeSP = intcodeBP;
    return EXIT_SUCCESS;
}

int32_t IntcodeExit(void)
{
    free(intcodeBP);
    free(intcodeBackup);
    return EXIT_SUCCESS;
}

int32_t IntcodeExec(INTCODE *input, size_t input_size)
{
    if (input == NULL || input_size == 0)
    {
        return EXIT_FAILURE;
    }
    
    // Consume opcodes until a non running status is returned.
    INTCODE_STATUS status = INTCODE_STATUS_RUNNING;
    intcodeSP = input;
    intcodeBP = input;
    intcodeOOB = input + input_size;
    while (status == INTCODE_STATUS_RUNNING && intcodeSP < intcodeOOB)
    {
        status = intcodeConsumeOpcode();
    }

    // A successful execution will return INTCODE_STATUS_HALT.
    return (status == INTCODE_STATUS_HALT) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int32_t IntcodeReverse(INTCODE *input, size_t input_size, INTCODE target)
{
    if (input == NULL || input_size == 0)
    {
        return EXIT_FAILURE;
    }

    // Brute force for now.
    for (INTCODE i = 0; i < input_size / sizeof(*input); ++i)
    {
        for (INTCODE j = 0; j < input_size / sizeof(*input); ++j)
        {
            int32_t status = IntcodeReset();
            if (status != EXIT_SUCCESS)
            {
                goto fail;
            }
            input[INTCODE_OFFSET_OP1] = i;
            input[INTCODE_OFFSET_OP2] = j;
            status = IntcodeExec(input, input_size);
            if (status != EXIT_SUCCESS)
            {
                goto fail;
            }
            if (input[0] == target)
            {
                return EXIT_SUCCESS;
            }
        }
    }

fail:
    return EXIT_FAILURE;
}

void IntcodePrintArray(INTCODE const *array, size_t size)
{
    if (array == NULL || size == 0)
    {
        return;
    }

    size_t array_size = size / sizeof(*array);
    if (array_size == 0)
    {
        return;
    }

    static char number[20] = {0};
    fputs("{", stdout);
    int32_t status = 0;
    while (array_size > 1)
    {
        --array_size;
        status = snprintf(number, sizeof(number), "%lu", *array++);
        if (status < 0)
        {
            return;
        }
        fputs(number, stdout);
        fputs(",", stdout);
    }

    status = snprintf(number, sizeof(number), "%lu", *array);
    if (status < 0)
    {
        return;
    }
    fputs(number, stdout);
    puts("}");
}