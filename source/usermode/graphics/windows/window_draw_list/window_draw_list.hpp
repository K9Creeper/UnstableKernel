/// --------------------
/// window_draw_list.hpp
/// @brief This file defines the WindowDrawList class, derived from the OrderedArray

#pragma once

#include "../../../../cpphelpers/ordered_array.hpp"

#define MAX_WINDOWS 256

class WindowDrawList : public OrderedArray<void *> {
    public:
        void FocusWindow(void* item);

        bool AddWindow(void* item, bool setUnder = false);
        void RemoveWindow(uint32_t i);

        void ForceSort();
};