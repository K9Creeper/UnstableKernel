#pragma once

#include "../../../cpphelpers/bitmap.hpp"

class PMMBitmap : public Bitmap<uint32_t> {
public:
  void Clear();
};