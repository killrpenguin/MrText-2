#pragma once

// Type used for storing tree metadata, such as byte and char length.
using Count = unsigned long;
using utf8Char = char8_t;

class RopeConst {
  public:
  const static int MaxStrLen;
  const static int Ten;
  const static int TempLastRow;
  const static int TempLineLen;
};
