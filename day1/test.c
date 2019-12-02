#include <FuelCounterUpper.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define VERBOSE
#define NUMBER_BASE (10)

static uint64_t testCount[] = {2u, 2u, 654u, 33583u};
static uint64_t testRequirements[] = {2u, 2u, 966u, 50346u};

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

    FILE *fp = fopen("./test.in", "r");
    if (fp == NULL)
    {
        puts("Failed to open input file.");
        return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int32_t status = EXIT_SUCCESS;
    uint32_t i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
#ifdef VERBOSE
        fputs("Adding module with mass: ", stdout);
        fputs(line, stdout);
#endif
        char *endPtr = NULL;
        uint64_t mass = strtoull(line, &endPtr, NUMBER_BASE);

        if (endPtr == NULL)
        {
            // Failed conversion.
            puts("Conversion failed.");
            status = -1;
            goto fail;
        }

        // Create test module
        MODULE module =
        {
            .mass = mass,
            .fuelRequirement = 0,
        };
        status = AddModule(&module);
        assert(status == 0);

        // Check calculated fuel requirement for test module and compare to expected result.
        uint64_t fuelCount = GetFuelCount();
#ifdef VERBOSE
        static char comparisonMessage[64] = {0};
        status = snprintf(comparisonMessage, sizeof(comparisonMessage), "Calculated: %lu, Expected: %lu\n", fuelCount, testCount[i]);
        if (status >= 0)
        {
            fputs(comparisonMessage, stdout);
        }
#endif
        assert(fuelCount == testCount[i]);
        uint64_t fuelRequirements = GetFuelRequirements();
#ifdef VERBOSE
        status = snprintf(comparisonMessage, sizeof(comparisonMessage), "Calculated: %lu, Expected: %lu\n", fuelRequirements, testRequirements[i]);
        if (status >= 0)
        {
            puts(comparisonMessage);
        }
#endif
        assert(fuelRequirements == testRequirements[i]);
        i++;

        // Reset fuel count.
        ResetFuelCount();
    }

fail:
    fclose(fp);
    if (line)
    {
        free(line);
    }

    return (status >= 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}