/// ---------------
/// wakeup_list.hpp
/// @brief This file defines the WakeupList class, which is dirived from the OrderedArray class.
/// even though it is derived from the *Ordered*Array class it is not going to utitialize anything
/// to do with ordering.

#pragma once

#include "wakeup.hpp"

#include "../../cpphelpers/ordered_array.hpp"

class WakeupList : public OrderedArray<void*>{
    public:
        bool Insert(Wakeup* w);
};