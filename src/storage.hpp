#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <vector>
#include <string>

/**
 * load paths of images in external folder
 * 
 * @return paths
 */
std::vector<std::string> load_external_paths();

/**
 * remove file at given path
 * will preserve file in trashbin folder
 * 
 * @param filePath path to file
 */
void remove_file(string filePath);

/**
 * move file at given path to another path
 * 
 * @param path of file
 * @param path to place
 */
void move_file(string from, string to);

#endif
