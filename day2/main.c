#include <Intcode.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char path[PATH_MAX];
    char *p;

    if(!(p = strrchr(argv[0], '/')))
    {
        getcwd(path, sizeof(path));
    }
    else
    {
        *p = '\0';
        chdir(argv[0]);
        getcwd(path, sizeof(path));
    }

    INTCODE *input = NULL;
    size_t input_size = 0;
    IntcodeInit(&input, &input_size);
    if (input == NULL || input_size == 0)
    {
        return EXIT_FAILURE;
    }

    fputs("Input: ", stdout);
    IntcodePrintArray(input, input_size);
    // Set input to the state before the 1202 program alarm.
    input[1] = 12u;
    input[2] = 2;
    int32_t status = IntcodeExec(input, input_size);
    assert(status == EXIT_SUCCESS);
    assert(input[0] == 4330636);
    fputs("Output: ", stdout);
    IntcodePrintArray(input, input_size);

    status = IntcodeReset();
    assert(status == EXIT_SUCCESS);

    status = IntcodeReverse(input, input_size, 19690720u);
    assert(status == EXIT_SUCCESS);

    uint64_t reverseCode = 100u * input[1] + input[2];
    assert(reverseCode == 6086);
    
    static char message[64] = {0};
    if (snprintf(message, sizeof(message), "Reverse Code: %lu", reverseCode) >= 0)
    {
        puts(message);
    }

    IntcodeExit();
    return status;
}