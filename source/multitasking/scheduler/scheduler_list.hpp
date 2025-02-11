/// ------------------
/// scheduler_list.hpp
/// @brief This file defines the SchedulerList class, which is derived from the OrderedArray class.

#pragma once

#include "../process/process.hpp"
#include "../../cpphelpers/ordered_array.hpp"

class SchedulerList : public OrderedArray<void*>{
    private:
        bool isPriority(Process* p1, Process* p2);
    public:
        bool Insert(Process* proc);
    private:
};