/// ------------------
/// scheduler_list.cpp
/// @brief This file defines the functions of the SchedulerList class.

#include "scheduler_list.hpp"

// If p1 is less priority than p2
bool SchedulerList::isPriority(Process *p1, Process *p2)
{
    return true;
}

bool SchedulerList::Insert(Process *proc)
{
    uint32_t i;

    static Process* tmp;
    static Process* tmp2;

    i = 0;

    while (i < this->size && this->isPriority(this->array[i], proc))
    {
        i++;
    }

    if (i + 1 > this->max_size)
        return false;

    if (i == this->size)
    {
        this->array[this->size] = proc;

        this->size++;
    }
    else // insert
    {
        tmp = this->array[i];

        this->array[i] = proc;

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