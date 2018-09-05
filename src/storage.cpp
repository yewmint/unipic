#include "storage.hpp"

#include <boost/filesystem.hpp>
#include <regex>
#include <iostream>
#include "fingerprint.hpp"

#define EXTERNAL_PATH "external"
#define STORAGE_PATH "storage"
#define TRASHBIN_PATH "trashbin"

#define IMAGE_PER_BUCKET 100
#define TRASHBIN_ID_MAX 100000

using namespace std;
using namespace boost::filesystem;

/**
 * load paths of images in external folder
 * 
 * @return paths
 */
vector<string> load_external_paths(){
  vector<string> imagePaths;
  regex imageRegex("\\.(png|jpg)$");

  if (!exists(EXTERNAL_PATH)){
    create_directories(EXTERNAL_PATH);
  }

  for (auto & it : recursive_directory_iterator(EXTERNAL_PATH)){
    if (is_regular_file(it.path())){
      string pathName = it.path().string();

      if (regex_search(pathName, imageRegex)){
        imagePaths.push_back(pathName);
      }
    }
  }

  return imagePaths;
}

/**
 * remove file at given path
 * will preserve file in trashbin folder
 * 
 * @param filePath path to file
 */
void remove_file(string filePath){
  path binPath = TRASHBIN_PATH;
  if (!exists(binPath)){
    create_directories(binPath);
  }

  if (!is_regular_file(filePath)){
    throw invalid_argument(
      string("ERROR in remove_file, file not existed: ") + filePath
    );
  }

  // "storate/009/10.jpg" => "009-10"
  int posLeft = filePath.find_first_of(path::preferred_separator);
  int posRight = filePath.find_last_of(path::dot);
  auto formerName = filePath.substr(posLeft + 1, posRight - posLeft - 1);
  for (char &ch : formerName){
    if (ch == path::preferred_separator) ch = '-';
  }

  srand(time(nullptr));
  string id = to_string(time(nullptr)) + to_string(rand());

  auto ext = path(filePath).extension().string();
  path toPath = binPath / path(formerName + "-"s + id + ext);

  rename(filePath, toPath);
}

/**
 * move file at given path to another path
 * 
 * @param path of file
 * @param path to place
 */
void move_file(string from, string to){
  if (!exists(from)){
    throw invalid_argument(
      string("ERROR in move_file, file not existed: ") + from
    );
  }

  if (!is_regular_file(from)){
    throw invalid_argument(
      string("ERROR in move_file, not regular file: ") + from
    );
  }

  path toFolder = path(to).remove_filename();
  if (!exists(toFolder)){
    create_directories(toFolder);
  }

  rename(from, to);
}

/**
 * get replace path if replace to with from
 * concat stem of from-path with extension of to-path
 * 
 * @param from
 * @param to
 * @return replace path
 */
string get_replace_path(string from, string to){
  int dotPos = to.find_last_of(path::dot);
  string stem = to.substr(0, dotPos);
  string newPath = stem + path(from).extension().string();
  return newPath;
}

/**
 * generate storage path from id
 * 
 * @param imagePath original path used to extract extension
 * @param id id of image
 * @return storage path for image
 */
string id_to_path(string imagePath, int id){
  int bucket = id / IMAGE_PER_BUCKET;
  int index = id % IMAGE_PER_BUCKET;

  char buffer[10];
  sprintf(buffer, "%03d", bucket);
  path folder = buffer;

  sprintf(buffer, "%02d", index);
  string name = buffer;

  string ext = path(imagePath).extension().string();
  path to = path(STORAGE_PATH) / folder / path(name + ext);

  return to.string();
}

#ifdef GENERATE_UNIT_TEST

#include <catch2/catch.hpp>

SCENARIO("Storage can load external images", "[storage_load_external]"){
  GIVEN("external images"){
    copy_file("chess-board.jpg", "external/00.jpg");
    vector<string> paths = load_external_paths();
    REQUIRE(paths.size() == 1);
    remove("external/00.jpg");
  }
}

SCENARIO("Storage can remove and move back image", "[move_file, remove_file]"){
  GIVEN("an image"){
    copy_file("chess-board.jpg", "00.jpg");
    move_file("00.jpg", "storage/000/00.jpg");
    remove_file("storage/000/00.jpg");
  }
}

SCENARIO("Storage can get replace path", "[get_replace_path]"){
  GIVEN("2 path"){
    auto str = get_replace_path("a/b/c.jpg", "0/0/0.png");
    REQUIRE(str == "0/0/0.jpg");
  }
}

SCENARIO("Storage can get path from id", "[id_to_path]"){
  GIVEN("2 path"){
    auto str = id_to_path("kkk.jpg", 0);
    REQUIRE(str == "storage/000/00.jpg");
  }
}

#endif
