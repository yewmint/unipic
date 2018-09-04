#ifndef __DB_H__
#define __DB_H__

#include <vector>
#include <sqlite3.h>
#include "image.hpp"

/**
 * get all informations in database
 * 
 * @return all informations
 */
std::vector<ImageInfo> storage_infos();

/**
 * update fingerprint and pixels by path
 * 
 * @param fp fingerprint
 * @param pixels
 * @param path path to image
 */
void update_info_by_path(fingerprint_t fp, int pixels, std::string path);

/**
 * insert new information into database
 * 
 * @param fp fingerprint
 * @param pixels
 * @param path path to image
 */
void insert_info(fingerprint_t fp, int pixels, string path);

/**
 * remove information from database by path
 *
 * @param path path to image
 */
void remove_info_by_path(string path);

#endif
