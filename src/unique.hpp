#ifndef __UNIQUE_H__
#define __UNIQUE_H__

#include <vector>
#include "image.hpp"

/**
 * replacing mark
 * replace index with info
 */
struct ReplacePair {
  int index;
  ImageInfo info;
};

/**
 * return unique images within array
 * 
 * @param infos image array which may contains duplicates
 * @return unique images
 */
std::vector<ImageInfo> unique_within(std::vector<ImageInfo>);

/**
 * return replace pairs between images in storage and external images
 * and new image informations
 * 
 * @param infos images in storage
 * @param others external images
 * @param replaces [out] replace pairs
 * @param newInfos [out] new informations not similar with existing ones
 */
void unique_merge(
  std::vector<ImageInfo> infos, 
  std::vector<ImageInfo> others,
  std::vector<ReplacePair> &replaces,
  std::vector<ImageInfo> &newInfos
);

#endif
