/// ----------
/// wakeup.hpp
/// @brief This file defines the wakeup structure.
#pragma once

#include <stdint.h>

typedef void (*wakeup_calback)();

struct Wakeup{
    void* callback;
    double seconds;
    uint32_t ticks;
};