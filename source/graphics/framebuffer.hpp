/// ------------
/// framebuffer.hpp
/// @brief This file declares the Framebuffer class and it's attributes. The class is used to manipulate linear framebuffers.

#include <stdint.h>

template <typename type_t>
class Framebuffer
{
protected:
    type_t *data;
public:
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;

    Framebuffer();

    uint32_t GetDataLocation();

    type_t& operator[](uint32_t i);

    type_t& Get(uint32_t i);    

    void RePlace(uint32_t address);

    void Swap(Framebuffer<type_t> &other);

private:
};

template class Framebuffer<uint32_t>;