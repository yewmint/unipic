#ifndef __UNIQUE_H__
#define __UNIQUE_H__

#include <vector>
#include "image.hpp"

struct ReplacePair {
  int fromIndex;
  int toIndex;
};

std::vector<ImageInfo> unique_within(std::vector<ImageInfo>);

#endif
