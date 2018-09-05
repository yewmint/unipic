#include "unique.hpp"

#include <mutex>
#include <tbb/tbb.h>

using namespace std;
using tbb::parallel_for;

/**
 * check if should ImageInfo b replace ImageInfo a
 * 
 * @param ImageInfo a
 * @param ImageInfo b
 * @return should replace
 */
bool should_B_replace_A(const ImageInfo &a, const ImageInfo &b){
  return is_similar(b.fingerprint, a.fingerprint) &&
    b.pixels > a.pixels;
}

/**
 * return unique images within array
 * 
 * @param infos image array which may contains duplicates
 * @return unique images
 */
vector<ImageInfo> unique_within(vector<ImageInfo> infos){
  vector<ImageInfo> uniqueInfos;
  
  for (auto &info : infos){
    bool foundDuplicate = false;

    for (auto &uniqueInfo : uniqueInfos){
      if (is_similar(info.fingerprint, uniqueInfo.fingerprint)){
        foundDuplicate = true;

        if (uniqueInfo.pixels < info.pixels){
          uniqueInfo = info;
        }
        
        break;
      }
    }

    if (!foundDuplicate) {
      uniqueInfos.push_back(info);
    }
  }

  return uniqueInfos;
}

/**
 * return replace pairs between images in storage and external images
 * and new image informations
 * 
 * @param infos images in storage
 * @param others external images
 * @param replaces [out] replace pairs
 * @param newInfos [out] new informations not similar with existing ones
 */
void unique_merge(
  std::vector<ImageInfo> infos, 
  std::vector<ImageInfo> others,
  std::vector<ReplacePair> &replaces,
  std::vector<ImageInfo> &newInfos
){
  mutex replaceMutex, newInfoMutex;

  parallel_for(size_t(0), others.size(), 
  [&replaces, &replaceMutex, &newInfos, &newInfoMutex, &infos, &others]
  (int otherIndex){
    ImageInfo &curInfo = others[otherIndex];
    bool foundDuplicate = false;

    for (int infoIndex = 0; size_t(infoIndex) < infos.size(); ++infoIndex){
      ImageInfo &info = infos[infoIndex];
      
      if (is_similar(info.fingerprint, curInfo.fingerprint)){
        foundDuplicate = true;

        if (curInfo.pixels > info.pixels){
          lock_guard guard(replaceMutex);
          replaces.push_back({ infoIndex, curInfo });
          foundDuplicate = true;
        }

        break;
      }
    }

    if (!foundDuplicate){
      lock_guard guard(newInfoMutex);
      newInfos.push_back(curInfo);
    }
  });
}

#ifdef GENERATE_UNIT_TEST

#include <catch2/catch.hpp>

SCENARIO("unique_within can get unique items within ImageInfo array"){
  GIVEN("An ImageInfo array"){
    ImageInfo a;
    a.fingerprint = 0x55aa55aa55aa55aaull;
    a.pixels = 10;
    a.path = "a";

    ImageInfo b;
    b.fingerprint = 0x75aa55aa55aa55aaull;
    b.pixels = 11;
    b.path = "b";

    vector<ImageInfo> infos = { a, b };
    auto unique = unique_within(infos);

    REQUIRE(unique.size() == 1);
    REQUIRE(unique[0].pixels == 11);
  }
}

SCENARIO("unique_merge can get replace pairs"){
  GIVEN("An ImageInfo array"){
    ImageInfo a;
    a.fingerprint = 0x55aa55aa55aa55aaull;
    a.pixels = 10;
    a.path = "a";

    ImageInfo b;
    b.fingerprint = 0x75aa55aa55aa55aaull;
    b.pixels = 11;
    b.path = "b";

    ImageInfo c;
    c.fingerprint = 0xaa55aa55aa55aa55ull;
    c.pixels = 100;
    c.path = "c";

    vector<ImageInfo> infos = { a };
    vector<ImageInfo> others = { b, c };
    vector<ReplacePair> replaces;
    vector<ImageInfo> newInfos;
    unique_merge(infos, others, replaces, newInfos);

    REQUIRE(replaces.size() == 1);
    REQUIRE(replaces[0].index == 0);

    REQUIRE(newInfos.size() == 1);
    REQUIRE(newInfos[0].pixels == 100);
  }
}

#endif
