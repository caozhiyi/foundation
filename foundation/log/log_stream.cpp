// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#include "foundation/log/log.h"
#include "foundation/log/log_stream.h"
#include "foundation/log/logger_interface.h"

namespace fdan {

#define CHECK_CONTINUE() \
  do { if (!log_ || log_->len >= __log_block_size) { return *this; } } while (0);

LogStream::LogStream(const LogStreamParam& param):
  log_(param.first),
  call_back_(param.second) {}

LogStream::~LogStream() {
  if (log_ && call_back_) {
    call_back_(log_);
  }
}

LogStream& LogStream::operator<<(bool v) {
  CHECK_CONTINUE()

  char c = v ? '1' : '0';
  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%c", c);
  return *this;
}

LogStream& LogStream::operator<<(int8_t v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%d", v);
  return *this;
}

LogStream& LogStream::operator<<(uint8_t v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%u", v);
  return *this;
}

LogStream& LogStream::operator<<(int16_t v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%d", v);
  return *this;
}

LogStream& LogStream::operator<<(uint16_t v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%u", v);
  return *this;
}

LogStream& LogStream::operator<<(int32_t v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%d", v);
  return *this;
}

LogStream& LogStream::operator<<(uint32_t v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%u", v);
  return *this;
}

LogStream& LogStream::operator<<(int64_t v) {
  CHECK_CONTINUE()
#ifdef __win__
  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%I64d", v);
#else
  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%ld", v);
#endif
  return *this;
}

LogStream& LogStream::operator<<(uint64_t v) {
  CHECK_CONTINUE()
#ifdef __win__
  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%I64u", v);
#else
  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%lu", v);
#endif
  return *this;
}

LogStream& LogStream::operator<<(float v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%.10lf", v);
  return *this;
}

LogStream& LogStream::operator<<(double v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%.20lf", v);
  return *this;
}

LogStream& LogStream::operator<<(const std::string& v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%s", v.c_str());
  return *this;
}

LogStream& LogStream::operator<<(const char* v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%s", v);
  return *this;
}

LogStream& LogStream::operator<<(char v) {
  CHECK_CONTINUE()

  log_->len += snprintf(log_->log + log_->len,
    __log_block_size - log_->len, "%c", v);
  return *this;
}

}  // namespace fdan
