#include "fingerprint.hpp"

/**
 * get number of ones in binary form
 * used in hamming distance
 * 
 * @param fp 64 bits integer
 * @return number of ones in binary form
 */
int number_of_ones_in_binary(fingerprint_t fp){
  int ret = 0;
  while (fp > 0){
    fp &= (fp - 1);
    ret++;
  }
  return ret;
}

/**
 * get hamming distance of 2 fingerprints
 * 
 * @param fpa 64 bits integer a
 * @param fpb 64 bits integer b
 * @return hamming distance
 */
int hamming_distance(fingerprint_t fpa, fingerprint_t fpb){
  return number_of_ones_in_binary(fpa ^ fpb);
}

/**
 * check if 2 fingerprints are similar
 * according to hamming distance between them
 * 
 * @param fpa
 * @param fpb
 * @return if they are similar
 */
int isSimilar(fingerprint_t fpa, fingerprint_t fpb){
  return hamming_distance(fpa, fpb) < SIMILAR_THRESHOLD;
}

#ifdef GENERATE_UNIT_TEST

#include "catch.hpp"

SCENARIO("Get number of ones in binary form of fingerprint", "[fingerprint_t]"){
  GIVEN("some fingerprints"){
    REQUIRE(number_of_ones_in_binary(0x0000000000000000ull) == 0);
    REQUIRE(number_of_ones_in_binary(0x0000000000000001ull) == 1);
    REQUIRE(number_of_ones_in_binary(0x8000000000000000ull) == 1);
    REQUIRE(number_of_ones_in_binary(0xffffffffffffffffull) == 64);
    REQUIRE(number_of_ones_in_binary(0xaaaaaaaaaaaaaaaaull) == 32);
    REQUIRE(number_of_ones_in_binary(0x5555555555555555ull) == 32);
  }
}

SCENARIO("Hamming distance can be calculated", "[fingerprint_t]") {
  GIVEN("2 fingerprints"){
    REQUIRE(hamming_distance(
      0x0000000000000000ull,
      0x0000000000000001ull
    ) == 1);

    REQUIRE(hamming_distance(
      0x0000000000000001ull,
      0x8000000000000000ull
    ) == 2);

    REQUIRE(hamming_distance(
      0xaaaaaaaaaaaaaaaaull,
      0x5555555555555555ull
    ) == 64);
  }
}

SCENARIO("Fingerprints can be compared", "[fingerprint_t]") {
  GIVEN("2 fingerprints"){
    REQUIRE(isSimilar(
      0x0000000000000000ull,
      0x0000000000000001ull
    ) == true);

    REQUIRE(isSimilar(
      0x0000000000000001ull,
      0x8000000000000000ull
    ) == true);

    REQUIRE(isSimilar(
      0xaaaaaaaaaaaaaaaaull,
      0x5555555555555555ull
    ) == false);
  }
}

#endif
