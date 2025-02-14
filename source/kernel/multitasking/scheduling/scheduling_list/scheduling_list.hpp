/// -------------------
/// scheduling_list.hpp
/// @brief This file declares and defines the SchedulingList class, 
// which is derived from the OrderedArray class.

#pragma once

#include "../../task/task.hpp"

#include "../../../../cpphelpers/ordered_array.hpp"

class SchedulingList : public OrderedArray<void*>{
    public: 
        bool Insert(Task* t);
    private:
        bool isPriority(Task* a, Task* b);
};