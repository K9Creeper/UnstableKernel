///
///
/// @brief This file declares and defines (the functions of) the class TimerArray,
/// which is specifically used by the system timer. It uses the 'Ordered Array' as a normal array.

#pragma once

#include "../../../cpphelpers/ordered_array.hpp"

class TimerArray : public OrderedArray<void*>{
    bool lessthan_(void *a, void *b);
};