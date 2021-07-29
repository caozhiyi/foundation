// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_LOG_FILE_LOGGER_H_
#define FOUNDATION_LOG_FILE_LOGGER_H_

#include <mutex>
#include <queue>
#include <memory>
#include <string>
#include <fstream>

#include "foundation/log/logger_interface.h"
#include "foundation/thread/thread_with_queue.h"

namespace fdan {

static const uint8_t kFileLoggerTimeBufLength = sizeof("xxxx-xx-xx:xx");

enum FileLoggerSpiltUnit {
  FLSU_DAY  = 1,
  FLSU_HOUR = 2,
};

class FileLogger:
  public Logger,
  public ThreadWithQueue<std::shared_ptr<Log>> {
 public:
  FileLogger(const std::string& file,
    FileLoggerSpiltUnit unit = FLSU_DAY,
    uint16_t max_store_days = 3,
    uint16_t time_offset = 5);

  ~FileLogger();

  void Run();
  void Stop();

  void Debug(const std::shared_ptr<Log>& log);
  void Info(const std::shared_ptr<Log>& log);
  void Warn(const std::shared_ptr<Log>& log);
  void Error(const std::shared_ptr<Log>& log);
  void Fatal(const std::shared_ptr<Log>& log);

  void SetFileName(const std::string& name) { file_name_ = name; }
  std::string GetFileName() { return file_name_; }

  void SetMaxStoreDays(uint16_t max);
  uint16_t GetMAxStorDays() { return max_file_num_; }

 private:
  void CheckTime(char* log);
  void CheckExpireFiles();

 private:
  std::string   file_name_;
  std::fstream  stream_;

  // for time check
  uint16_t time_offset_;
  uint8_t  time_buf_len_;
  FileLoggerSpiltUnit spilt_unit_;
  char   time_[kFileLoggerTimeBufLength];

  // for log file delete
  uint16_t max_file_num_;
  std::queue<std::string> history_file_names_;
};

}  // namespace fdan

#endif  // FOUNDATION_LOG_FILE_LOGGER_H_
