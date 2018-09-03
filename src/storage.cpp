#include "storage.hpp"

#include <boost/filesystem.hpp>
#include <regex>
#include <iostream>

#define EXTERNAL_PATH "external"

using namespace std;
using namespace boost::filesystem;

vector<string> storage_load_external(){
  vector<string> imagePaths;
  regex imageRegex("\\.(png|jpg)$");

  for (auto & it : recursive_directory_iterator(EXTERNAL_PATH)){
    string subpath = it.path().string();
    cout << subpath << endl;
    if (regex_match(subpath, imageRegex)){
      imagePaths.push_back(subpath);
    }
  }

  return imagePaths;
}

#ifdef GENERATE_UNIT_TEST

#include "catch.hpp"

SCENARIO("Storage can load external images", "[storage_load_external]"){
  GIVEN("external images"){
    vector<string> paths = storage_load_external();
    REQUIRE(paths.size() == 1);
  }
}

#endif
