// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_LOG_LOG_STREAM_H_
#define FOUNDATION_LOG_LOG_STREAM_H_

#include <memory>
#include <string>
#include <cstdint>
#include <utility>
#include <functional>

namespace fdan {

struct Log;
typedef std::pair<std::shared_ptr<Log>, std::function<void(std::shared_ptr<Log>)>> LogStreamParam;

class LogStream {
 public:
  explicit LogStream(const LogStreamParam& param);
  ~LogStream();

  LogStream& Stream() { return *this; }

  LogStream& operator<<(bool v);
  LogStream& operator<<(int8_t v);
  LogStream& operator<<(uint8_t v);
  LogStream& operator<<(int16_t v);
  LogStream& operator<<(uint16_t v);
  LogStream& operator<<(int32_t v);
  LogStream& operator<<(uint32_t v);
  LogStream& operator<<(int64_t v);
  LogStream& operator<<(uint64_t v);
  LogStream& operator<<(float v);
  LogStream& operator<<(double v);
  LogStream& operator<<(const std::string& v);
  LogStream& operator<<(const char* v);
  LogStream& operator<<(char v);

 private:
  LogStream(const LogStream&) = delete;
  LogStream& operator=(const LogStream&) = delete;

 private:
  std::shared_ptr<Log> log_;
  std::function<void(std::shared_ptr<Log>)> call_back_;
};

}  // namespace fdan

#endif  // FOUNDATION_LOG_LOG_STREAM_H_
