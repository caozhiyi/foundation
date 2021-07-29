// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_LOG_LOGGER_INTERFACE_H_
#define FOUNDATION_LOG_LOGGER_INTERFACE_H_

#include <memory>
#include <cstdint>

namespace fdan {

struct Log {
  char*    log;
  uint32_t len;
};

// inherit this class to print log.
// can set subclasses for different print combinations.
class Logger {
 public:
  Logger() {}
  virtual ~Logger() {}

  void SetLogger(std::shared_ptr<Logger> logger) { logger_ = logger; }
  std::shared_ptr<Logger> GetLogger() { return logger_; }

  virtual void Debug(const std::shared_ptr<Log>& log)
    { if (logger_) logger_->Debug(log); }
  virtual void Info(const std::shared_ptr<Log>& log)
    { if (logger_) logger_->Info(log); }
  virtual void Warn(const std::shared_ptr<Log>& log)
    { if (logger_) logger_->Warn(log); }
  virtual void Error(const std::shared_ptr<Log>& log)
    { if (logger_) logger_->Error(log); }
  virtual void Fatal(const std::shared_ptr<Log>& log)
    { if (logger_) logger_->Fatal(log); }

 protected:
  std::shared_ptr<Logger> logger_;
};

}  // namespace fdan

#endif  // FOUNDATION_LOG_LOGGER_INTERFACE_H_
