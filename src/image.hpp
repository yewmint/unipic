#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include <map>
#include <vector>
#include "fingerprint.hpp"

/**
 * informations of image
 * 
 * @member pixels total pixels of image, representing resolution
 * @member fingerprint fingerprint of image
 */
struct ImageInfo {
  int pixels;
  fingerprint_t fingerprint;
};

/**
 * extract informations from image store at given path
 * 
 * @param path path to image
 * @return information of image
 */
ImageInfo image_info(std::string path);

/**
 * extract informations of images parallelly
 * 
 * @param paths paths of images
 * @return map of path and info
 */
std::map<std::string, ImageInfo>
image_infos_parallel(std::vector<std::string> paths);

#endif
