#include <FuelCounterUpper.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

static uint64_t fuelCount = 0;
static uint64_t fuelRequirements = 0;

static int32_t calculateFuelRequirement(MODULE *module)
{
    if (module == NULL || module->mass == 0)
    {
        return -1;
    }

    // To find the fuel required for a module, take its mass, divide by three, round down, and subtract 2.
    uint64_t fuelCalculation = module->mass / 3u; // NOTE: Integer division truncates (round down).
    module->fuelRequirement = (fuelCalculation >= 2u) ? (fuelCalculation - 2u) : 0;

    return 0;
}

int32_t AddModule(MODULE *module)
{
    if (module == NULL || module->mass == 0)
    {
        puts("Invalid module.");
        return -1;
    }

    int32_t status = calculateFuelRequirement(module);
    if (status != 0)
    {
        return status;
    }
    
    fuelCount += module->fuelRequirement;

    MODULE fuelModule = 
    {
        .mass = module->fuelRequirement,
        .fuelRequirement = 1,
    };

    while (fuelModule.fuelRequirement != 0)
    {
        fuelModule.fuelRequirement = 0;
        status = calculateFuelRequirement(&fuelModule);
        if (status != 0)
        {
            return status;
        }
        module->fuelRequirement += fuelModule.fuelRequirement;
        fuelModule.mass = fuelModule.fuelRequirement;
    }

    fuelRequirements += module->fuelRequirement;

    return status;
}

uint64_t GetFuelCount(void)
{
    return fuelCount;
}

uint64_t GetFuelRequirements(void)
{
    return fuelRequirements;
}

void ResetFuelCount(void)
{
    fuelCount = 0;
    fuelRequirements = 0;
}