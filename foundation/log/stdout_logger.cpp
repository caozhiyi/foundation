// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include <iostream>
#include "foundation/log/stdout_logger.h"

namespace fdan {

StdoutLogger::StdoutLogger() {}

StdoutLogger::~StdoutLogger() {}

void StdoutLogger::Debug(const std::shared_ptr<Log>& log) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cout<< log->log << std::endl;
  }
  Logger::Debug(log);
}

void StdoutLogger::Info(const std::shared_ptr<Log>& log) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cout<< log->log << std::endl;
  }
  Logger::Info(log);
}

void StdoutLogger::Warn(const std::shared_ptr<Log>& log) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cout<< log->log << std::endl;
  }
  Logger::Warn(log);
}

void StdoutLogger::Error(const std::shared_ptr<Log>& log) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cerr<< log->log << std::endl;
  }
  Logger::Error(log);
}

void StdoutLogger::Fatal(const std::shared_ptr<Log>& log) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cerr<< log->log << std::endl;
  }
  Logger::Fatal(log);
}

}  // namespace fdan
