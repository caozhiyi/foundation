// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_LOG_STDOUT_LOGGER_H_
#define FOUNDATION_LOG_STDOUT_LOGGER_H_

#include <mutex>
#include <memory>

#include "foundation/log/logger_interface.h"

namespace fdan {

class StdoutLogger:
  public Logger {
 public:
  StdoutLogger();
  ~StdoutLogger();

  void Debug(const std::shared_ptr<Log>& log);
  void Info(const std::shared_ptr<Log>& log);
  void Warn(const std::shared_ptr<Log>& log);
  void Error(const std::shared_ptr<Log>& log);
  void Fatal(const std::shared_ptr<Log>& log);

 private:
  std::mutex mutex_;
};

}  // namespace fdan

#endif  // FOUNDATION_LOG_STDOUT_LOGGER_H_
