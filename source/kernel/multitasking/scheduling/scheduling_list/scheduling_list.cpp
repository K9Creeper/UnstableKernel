/// -------------------
/// scheduling_list.cpp
/// @brief This file defines the functions of the SchedulingList class.

#include "scheduling_list.hpp"

// Work on this...
// If a is less priority than b
bool SchedulingList::isPriority(Task* a, Task* b){
    return false;
}

bool SchedulingList::Insert(Task* t)
{
    uint32_t i;

    static Task* tmp;
    static Task* tmp2;

    i = 0;

    while (i < this->size && this->isPriority(this->array[i], t))
    {
        i++;
    }

    if (i + 1 > this->max_size)
        return false;

    if (i == this->size)
    {
        this->array[this->size] = t;

        this->size++;
    }
    else // insert
    {
        tmp = this->array[i];

        this->array[i] = t;

        while (i < this->size)
        {
            i++;

            tmp2 = this->array[i];

            this->array[i] = tmp;

            tmp = tmp2;
        }

        this->size++;
    }

    return true;
}

