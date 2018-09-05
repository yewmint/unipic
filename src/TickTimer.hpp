#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>

/**
 * To get duration in microseconds between ticks
 */
class TickTimer{
public:
  /**
   * @constructor
   */
  TickTimer();

  /**
   * return microseconds since last tick or timer constructing
   * 
   * @return microseconds
   */
  double tick();

private:
  /**
   * save last tick time point
   */
  std::chrono::time_point<std::chrono::high_resolution_clock> _last;
};

#endif
