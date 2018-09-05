#include "logger.hpp"

#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>

using namespace std;

#define LOGGER_FILE "log"

shared_ptr<spdlog::logger> _logger;
bool _inited = false;

void init_log() {
  if (!_inited){
    _logger = spdlog::basic_logger_mt("basic_logger", LOGGER_FILE);
    _logger->set_pattern("[%H:%M:%S %z] [thread %t] %v");
    _inited = true;
  }
}

void log_info(string msg){
  init_log();
  _logger->info(msg);
}
