#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__

typedef unsigned long long fingerprint_t;

int numberOfOneInBinary(fingerprint_t fp){
  int ret = 0;
  while (fp > 0){
    fp &= (fp - 1);
    ret++;
  }
  return fp;
}

#endif
