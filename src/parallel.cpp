#include "parallel.hpp"

#include <thread>
#include <stdexcept>
#include <mutex>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace std::chrono_literals;

#define MAIN_THREAD_WAIT_TIME 10ms

/**
 * run for loop in parallel
 * 
 * @param length loop i in range [0, length - 1]
 * @param func func will be invoked like func(i)
 */
void parallel_for(int length, std::function<void(int)> func){
  // loop index in range [0, length - 1]
  int index = 0;
  mutex indexMutex;

  // number of finished threads
  int finishNum = 0;
  mutex finishNumMutex;

  auto threadFunc = [
    &index, &indexMutex, &finishNum, &finishNumMutex, &func, length](){
    while (true){
      int curIndex = -1;
      {
        lock_guard guard(indexMutex);
        if (index >= length) break;
        curIndex = index;
        index++;
      }

      try {
        func(curIndex);
      }
      catch (exception e){
        cout << "ERROR in parallel_for: " << e.what() << endl;
      }
    }

    lock_guard guard(finishNumMutex);
    finishNum++;
  };

  int threadNum = min<int>(length + 1, thread::hardware_concurrency());
  for (int i = 0; i < threadNum; ++i){
    thread th(threadFunc);
    th.detach();
  }

  // wait for threads to finish
  while (finishNum != threadNum){
    this_thread::sleep_for(MAIN_THREAD_WAIT_TIME);
  }
}

#ifdef GENERATE_UNIT_TEST

#include <catch.hpp>

TEST_CASE("Function should run in parallel", "[parallel_for]"){
  SECTION("a function"){
    vector<int> input({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    vector<int> output;
    mutex outputMutex;

    auto func = [&input, &output, &outputMutex](int i) -> void {
      int num = input[i];
      lock_guard guard(outputMutex);
      output.push_back(num);
    };

    parallel_for(10, func);

    REQUIRE(output.size() == 10);
  }
}

#endif
