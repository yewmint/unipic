#include "unique.hpp"

#include <mutex>
#include <tbb/tbb.h>

using namespace std;
using tbb::parallel_for;

bool should_B_replace_A(const ImageInfo &a, const ImageInfo &b){
  return is_similar(b.fingerprint, a.fingerprint) &&
    b.pixels > a.pixels;
}

vector<ImageInfo> unique_within(vector<ImageInfo> infos){
  vector<ImageInfo> uniqueInfos;
  
  for (auto &info : infos){
    bool replaced = false;

    for (auto &uniqueInfo : uniqueInfos){
      if (should_B_replace_A(uniqueInfo, info)){
        uniqueInfo = info;
        replaced = true;
        break;
      }
    }

    if (!replaced) {
      uniqueInfos.push_back(info);
    }
  }

  return uniqueInfos;
}

vector<ReplacePair> 
unique_merge(vector<ImageInfo> infos, vector<ImageInfo> others){
  vector<ReplacePair> replaces;
  mutex replaceMutex;

  parallel_for(size_t(0), others.size(), 
  [&replaces, &replaceMutex, &infos, &others](int otherIndex){
    ImageInfo &curInfo = others[otherIndex];

    for (int infoIndex = 0; infoIndex < infos.size(); ++infoIndex){
      ImageInfo &info = infos[infoIndex];
      if (should_B_replace_A(info, curInfo)){
        lock_guard guard(replaceMutex);
        replaces.push_back({ otherIndex, infoIndex });
        break;
      }
    }
  });

  return replaces;
}

#ifdef GENERATE_UNIT_TEST

#include <catch.hpp>

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

    vector<ImageInfo> infos = { a };
    vector<ImageInfo> others = { b };
    auto replaces = unique_merge(infos, others);

    REQUIRE(replaces.size() == 1);
    REQUIRE(replaces[0].fromIndex == 0);
    REQUIRE(replaces[0].toIndex == 0);
  }
}

#endif

