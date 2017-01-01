#pragma once

#include "fermch.h"
#include <core.h>

void setupTempe();

inline int16_t readTempe() {
  return (int16_t)core::store::analogs[idTempe];
}


