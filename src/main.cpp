#ifdef GENERATE_UNIT_TEST

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#else

#include "system.hpp"

int main(){
  system_run();
}

#endif