#include "TickTimer.hpp"

using namespace std;
using namespace std::chrono;

/**
 * @constructor
 */
TickTimer::TickTimer(){
  this->_last = high_resolution_clock::now();
}

/**
 * return microseconds since last tick or timer constructing
 * 
 * @return microseconds
 */
double TickTimer::tick(){
  auto nowPoint = high_resolution_clock::now();
  auto duration = nowPoint - this->_last;
  this->_last = nowPoint;
  return duration.count() / 1e6;
}
