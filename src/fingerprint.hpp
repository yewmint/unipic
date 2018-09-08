#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__

/**
 * 2 fingerprints are considered dissimilar with each other
 * if hamming distance between them exceeds SIMILAR_THRESHOLD
 */
#define SIMILAR_THRESHOLD 2

/**
 * 64 bits integer
 * @type fingerprint_t
 */
typedef unsigned long long fingerprint_t;

/**
 * get number of ones in binary form
 * used in hamming distance
 * 
 * @param fp 64 bits integer
 * @return number of ones in binary form
 */
int numberOfOneInBinary(fingerprint_t fp);

/**
 * get hamming distance of 2 fingerprints
 * 
 * @param fpa 64 bits integer a
 * @param fpb 64 bits integer b
 * @return hamming distance
 */
int hamming_distance(fingerprint_t fpa, fingerprint_t fpb);

/**
 * check if 2 fingerprints are similar
 * according to hamming distance between them
 * 
 * @param fpa
 * @param fpb
 * @return if they are similar
 */
int is_similar(fingerprint_t fpa, fingerprint_t fpb);

#endif
