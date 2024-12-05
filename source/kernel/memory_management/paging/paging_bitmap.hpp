#pragma once

#include "../../../cpphelpers/bitmap.hpp"

class PagingBitmap : public Bitmap<uint32_t> {
public:
  void Clear();
};