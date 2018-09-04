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

// path id_to_path(int id){
//   int bucket = (id - 1) / IMAGE_PER_BUCKET;
//   int index = (id - 1) % IMAGE_PER_BUCKET;

//   char buffer[10];
//   sprintf(buffer, "%03d", bucket);
//   path folder = buffer;

//   sprintf(buffer, "%02d", index);
//   string name = buffer;

//   string ext = path(imagePath).extension().string();
//   path from = imagePath;
//   path to = path(STORAGE_PATH) / folder / path(name + ext);
// }

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

#ifdef GENERATE_UNIT_TEST

#include "catch.hpp"

SCENARIO("Storage can load external images", "[storage_load_external]"){
  GIVEN("external images"){
    vector<string> paths = load_external_paths();
    REQUIRE(paths.size() == 1);
  }
}

SCENARIO("Storage can remove and move back image", "[move_file, remove_file]"){
  GIVEN("an image"){
    copy_file("chess-board.jpg", "00.jpg");
    move_file("00.jpg", "storage/000/00.jpg");
    remove_file("storage/000/00.jpg");
  }
}

#endif
