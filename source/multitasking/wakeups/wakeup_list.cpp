/// ---------------
/// wakeup_list.cpp
/// @brief This file defines the Insert function of the WakeupList class.

#include "wakeup_list.hpp"

bool WakeupList::Insert(Wakeup* wakeup)
{
    uint32_t i = 0;

    while(i < this->size && !this->array[i]){
        i++;
        if(i > this->max_size)
            return false;
    }

    this->array[i] = wakeup;
    this->size++;

    return true;
}