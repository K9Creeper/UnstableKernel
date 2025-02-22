/// --------------------
/// window_draw_list.cpp
/// @brief This file defines the functions for keeping track
/// and keeping order of the rendering windows.

#include "window_draw_list.hpp"

#include "../window/window.hpp"

void WindowDrawList::FocusWindow(void * w){
    Usermode::Graphics::Windows::Window* window = w;
    if(window)
        window->zindex = 99999;

    shouldBeSorted = true;
}

bool WindowDrawList::AddWindow(void *w)
{
    Usermode::Graphics::Windows::Window* window = w;

    if (this->size + 1 > this->max_size)
        return false;

    uint32_t i = 0;
    while (i < this->size)
    {
        if (array[i] == window)
            return false;
        i++;
    }

    // just append the item, it'll be sorted...eventually.
    {
        this->array[i] = window;

        this->size++;

        shouldBeSorted = true;
    }

    return true;
}

void WindowDrawList::RemoveWindow(uint32_t i){
    Remove(i);
    shouldBeSorted = true;
}

void WindowDrawList::ForceSort()
{
    if(size <= 1)
        return;
    bool sort = true;
    while (sort)
    {
        sort = false;
        for (int16_t i = size - 1; i > 0; i--)
        {
            Usermode::Graphics::Windows::Window *w1 = array[i];
            Usermode::Graphics::Windows::Window *w2 = array[i - 1];

            if (!w1 || !w2)
            {
                if (!w1)
                {
                    Remove(i);
                    i++;
                }
                if (!w2)
                {
                    Remove(i - 1);
                    i++;
                }

                if (i - 1 >= size)
                    break;

                sort = true;
                continue;
            }

            if (w1->zindex > w2->zindex)
            {
                array[i] = w1;
                array[i - 1] = w2;
                sort = true;
            }
        }
    }

    // Make sure z-indexes are seq
    for (int16_t i = 0; i < size; i++)
    {
        if (array[i])
        {
            reinterpret_cast<Usermode::Graphics::Windows::Window *>(array[i])->zindex = i;
        }
    }

    shouldBeSorted = false;
}
