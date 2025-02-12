/// ----------
/// wakeup.hpp
/// @brief This file defines the wakeup structure.
#pragma once

#include <stdint.h>

#include "../../chelpers/registers.h"

typedef void (*wakeup_calback)(Registers* reg);

struct Wakeup{
    void* callback;
    double seconds;
    uint32_t ticks;
};