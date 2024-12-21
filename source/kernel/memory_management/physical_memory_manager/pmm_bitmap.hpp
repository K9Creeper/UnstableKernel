#pragma once

#include "../../../cpphelpers/bitmap.hpp"

template class Bitmap<uint32_t>;

class PMMBitmap : public Bitmap<uint32_t> {
public:
  void Clear();
};