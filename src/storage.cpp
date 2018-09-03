#include "storage.hpp"

#include <boost/filesystem.hpp>
#include <regex>
#include <iostream>
#include <sstream>
#include <sqlite3.h>
#include "fingerprint.hpp"

#define EXTERNAL_PATH "external"
#define STORAGE_PATH "storage"
#define IMAGE_PER_BUCKET 100

using namespace std;
using namespace boost::filesystem;

vector<string> load_external_paths(){
  vector<string> imagePaths;
  regex imageRegex("\\.(png|jpg)$");

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

void external_to_storage(string imagePath, int id){

  // TODO: rename
  // TODO: extract path generator
}

#ifdef GENERATE_UNIT_TEST

#include "catch.hpp"

SCENARIO("Storage can load external images", "[storage_load_external]"){
  GIVEN("external images"){
    vector<string> paths = load_external_paths();
    REQUIRE(paths.size() == 1);
  }

  sqlite3 *db;
  sqlite3_open("b.db", &db);
  sqlite3_close(db);
}

#endif
