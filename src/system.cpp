#include "system.hpp"

#include "storage.hpp"
#include "image.hpp"
#include "unique.hpp"
#include "db.hpp"

using namespace std;

/**
 * system start point
 */
void system_run(){
  // 1. get unique external image informations
  auto externalPaths = load_external_paths();
  auto externalInfos = image_infos_parallel(externalPaths);
  auto externalUniqueInfos = unique_within(externalInfos);

  // 2. get storage image informations
  auto storageInfos = storage_infos();

  // 3. get image-replace pairs and new image informations
  vector<ReplacePair> replacePairs;
  vector<ImageInfo> newImageInfos;
  unique_merge(storageInfos, externalUniqueInfos, replacePairs, newImageInfos);

  // 4. replace old image with higher resolution image
  for (const auto & replacePair : replacePairs){
    const auto & replaced = storageInfos[replacePair.index];
    const auto & replacing = replacePair.info;

    string newPath = get_replace_path(replacing.path, replaced.path);

    update_info_by_path(
      replacing.fingerprint, 
      replacing.pixels, 
      newPath, 
      replaced.path
    );
    
    remove_file(replaced.path);
    move_file(replacing.path, newPath);
  }

  // 5. insert new images
  int id = storageInfos.size();
  for (const auto & newImageInfo : newImageInfos){
    int curId = id++;
    string newPath = id_to_path(newImageInfo.path, curId);
    insert_info(newImageInfo.fingerprint, newImageInfo.pixels, newPath);
    move_file(newImageInfo.path, newPath);
  }
}

#ifdef GENERATE_UNIT_TEST

#include <catch.hpp>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

SCENARIO(
  "system run can find dulplicates and replace it with higher resolution"
  ){
  GIVEN("an empty storage"){
    clear_db();
    copy_file("chess-board.jpg", "external/00.jpg");
    system_run();
    copy_file("chess-board.jpg", "external/00.jpg");
    system_run();
    remove("storage/000/00.jpg");
    remove("external/00.jpg");
  }
}

#endif
