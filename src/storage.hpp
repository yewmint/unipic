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
void remove_file(std::string filePath);

/**
 * move file at given path to another path
 * 
 * @param path of file
 * @param path to place
 */
void move_file(std::string from, std::string to);

/**
 * get replace path if replace to with from
 * concat stem of from-path with extension of to-path
 * 
 * @param from
 * @param to
 * @return replace path
 */
std::string get_replace_path(std::string from, std::string to);

/**
 * generate storage path from id
 * 
 * @param imagePath original path used to extract extension
 * @param id id of image
 * @return storage path for image
 */
std::string id_to_path(std::string imagePath, int id);

#endif
