#pragma once

#include <stdint.h>

typedef struct
{
    uint64_t mass;
    uint64_t fuelRequirement;
} MODULE;

int32_t AddModule(MODULE *module);
uint64_t GetFuelCount(void);
uint64_t GetFuelRequirements(void);
void ResetFuelCount(void);