#pragma once

#include <stdint.h>

#include "kheap.hpp"


extern uint32_t kmalloc ( uint32_t size );
extern void kfree ( void *p );
