/// ------------------
/// scheduler_list.hpp
/// @breif This file declares the class SchedulerList.

#pragma once

#include "../../cpphelperes/ordered_array"

#include "../proccess/proccess.hpp"

class SchedulerList : public OrderedArray<Tasking::Proccess*>{
    private:
    bool lessthan(Tasking::Proccess* a, Tasking::Proccess* b);
}