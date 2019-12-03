#include <Intcode.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    static char const resultMessage[] = "Result: ";
    static char const expectedMessage[] = "Expected: ";

    {
        INTCODE input[] = {1u, 0u, 0u, 0u, 99u};
        INTCODE const expectedOutput[] = {2u, 0u, 0u, 0u, 99u};
        int32_t status = IntcodeExec(input, sizeof(input));
        assert(status == EXIT_SUCCESS);
        status = memcmp(input, expectedOutput, sizeof(input));
        assert(status == 0);

        fputs(resultMessage, stdout);
        IntcodePrintArray(input, sizeof(input));
        fputs(expectedMessage, stdout);
        IntcodePrintArray(expectedOutput, sizeof(expectedOutput));
    }

    {
        INTCODE input[] = {2u, 3u, 0u, 3u, 99u};
        INTCODE const expectedOutput[] = {2u, 3u, 0u, 6u, 99u};
        int32_t status = IntcodeExec(input, sizeof(input));
        assert(status == EXIT_SUCCESS);
        status = memcmp(input, expectedOutput, sizeof(input));
        assert(status == 0);

        fputs(resultMessage, stdout);
        IntcodePrintArray(input, sizeof(input));
        fputs(expectedMessage, stdout);
        IntcodePrintArray(expectedOutput, sizeof(expectedOutput));
    }

    {
        INTCODE input[] = {2u, 4u, 4u, 5u, 99u, 0u};
        INTCODE const expectedOutput[] = {2u, 4u, 4u, 5u, 99u, 9801u};
        int32_t status = IntcodeExec(input, sizeof(input));
        assert(status == EXIT_SUCCESS);
        status = memcmp(input, expectedOutput, sizeof(input));
        assert(status == 0);

        fputs(resultMessage, stdout);
        IntcodePrintArray(input, sizeof(input));
        fputs(expectedMessage, stdout);
        IntcodePrintArray(expectedOutput, sizeof(expectedOutput));
    }

    {
        INTCODE input[] = {1u, 1u, 1u, 4u, 99u, 5u, 6u, 0u, 99u};
        INTCODE const expectedOutput[] = {30u, 1u, 1u, 4u, 2u, 5u, 6u, 0u, 99u};
        int32_t status = IntcodeExec(input, sizeof(input));
        assert(status == EXIT_SUCCESS);
        status = memcmp(input, expectedOutput, sizeof(input));
        assert(status == 0);

        fputs(resultMessage, stdout);
        IntcodePrintArray(input, sizeof(input));
        fputs(expectedMessage, stdout);
        IntcodePrintArray(expectedOutput, sizeof(expectedOutput));
    }

    return EXIT_SUCCESS;
}