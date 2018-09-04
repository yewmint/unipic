#include "system.hpp"

#include "storage.hpp"
#include "image.hpp"
#include "unique.hpp"
#include "db.hpp"

using namespace std;

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

    remove_file()
  }
}
