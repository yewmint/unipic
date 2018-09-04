#include "image.hpp"

#include <stdexcept>
#include <functional>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <tbb/tbb.h>

using namespace std;
using namespace cv;
using tbb::parallel_for;

/**
 * extract number of pixels from image
 * 
 * @param img
 * @return number of pixels
 */
int image_pixels(const Mat & img){
  return img.rows * img.cols;
}

/**
 * extract fingerprint from image
 * steps:
 * 1. resize to 8x8
 * 2. convert to grayscale
 * 3. threshold with mean value to binary image
 * 4. restructure binary image into fingerprint
 * 
 * @param img
 * @return fingerprint
 */
fingerprint_t image_fingerprint(const Mat & img){
  Mat tinyImg, grayImg, binImg;
  resize(img, tinyImg, Size(8, 8));
  cvtColor(tinyImg, grayImg, COLOR_BGR2GRAY);
  Scalar avg = mean(grayImg);
  binImg = grayImg > avg[0];

  // read bits row by row, then col by col
  // write bits from lower to higher
  fingerprint_t fp = 0x0ull;
  for (int row = 0; row < 8; ++row){
    for (int col = 0; col < 8; ++col){
      if (binImg.at<uchar>(row, col) > 0){
        fp |= 0x1ull << (row * 8 + col);
      }
    }
  }

  return fp;
}

/**
 * extract informations from image store at given path
 * 
 * @param path path to image
 * @return information of image
 */
ImageInfo image_info(string path){
  Mat img = imread(path);

  if (img.data == NULL){
    throw invalid_argument(string("Can't open image: ") + path);
  }

  ImageInfo info;
  info.path = path;
  info.pixels = image_pixels(img);
  info.fingerprint = image_fingerprint(img);

  return info;
}

/**
 * extract informations of images parallelly
 * 
 * @param paths paths of images
 * @return map of path and info
 */
vector<ImageInfo> image_infos_parallel(vector<string> paths){
  vector<ImageInfo> infos;
  mutex infoMutex;

  parallel_for(size_t(0), paths.size(), [&infos, &infoMutex, &paths](int i) {
    string str = paths[i];
    ImageInfo info = image_info(str);
    lock_guard guard(infoMutex);
    infos.push_back(info);
  });

  return infos;
}

#ifdef GENERATE_UNIT_TEST

#include <catch.hpp>

SCENARIO("Image info can be acquired", "[ImageInfo]"){
  GIVEN("A 8x8 chess board image"){
    ImageInfo chess_info = image_info("chess-board.jpg");
    REQUIRE(chess_info.pixels == 6400);
    REQUIRE(chess_info.fingerprint == 0x55aa55aa55aa55aaull);
  }
}

SCENARIO("Image info can be acquired parallelly", "[ImageInfo]"){
  GIVEN("10 8x8 chess board images"){
    vector<string> paths(10, "chess-board.jpg");
    auto infos = image_infos_parallel(paths);
    REQUIRE(infos.size() == 10);
    REQUIRE(infos[0].fingerprint == 0x55aa55aa55aa55aaull);
  }
}

#endif
