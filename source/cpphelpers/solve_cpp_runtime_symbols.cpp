extern "C" void* __dso_handle = 0;
extern "C" int __cxa_atexit(void (*destructor)(void*), void* object, void* dso_handle) {
    return 0; // Do nothing
}