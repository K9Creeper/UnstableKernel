/// -----------------------------
/// solve_cpp_runtime_symbols.cpp
/// @brief This File defines & declares CPP Runtime Symbols, which cause compilation errors
/// due to the project being compiled in a freestanding enviorment.

extern "C" void* __dso_handle = 0;
extern "C" int __cxa_atexit(void (*destructor)(void*), void* object, void* dso_handle) {
    return 0; // Do nothing
}