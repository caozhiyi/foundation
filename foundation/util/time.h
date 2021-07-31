// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_UTIL_TIME_H_
#define FOUNDATION_UTIL_TIME_H_

#include <string>

namespace fdan {

static const uint8_t kFormatTimeBufSize = sizeof("xxxx-xx-xx:xx:xx:xx:xxx");

enum FormatTimeUnit {
  FTU_YEAR        = 1,  // 2021
  FTU_MONTH       = 2,  // 2021-03
  FTU_DAY         = 3,  // 2021-03-16
  FTU_HOUR        = 4,  // 2021-03-16:10
  FTU_MINUTE      = 5,  // 2021-03-16:10:03
  FTU_SECOND      = 6,  // 2021-03-16:10:03:33
  FTU_MILLISECOND = 7,  // 2021-03-16:10:03:33:258
};

// get format time string [xxxx-xx-xx xx:xx:xx]
std::string GetFormatTime(FormatTimeUnit unit = FTU_MILLISECOND);
// get format time string as [xxxx-xx-xx xx:xx:xx]
void GetFormatTime(char* buf, uint32_t& len,
  FormatTimeUnit unit = FTU_MILLISECOND);

// get utc time
uint64_t UTCTimeSec();
uint64_t UTCTimeMsec();

// sleep interval milliseconds
void Sleep(uint32_t interval);

}  // namespace fdan

#endif  // FOUNDATION_UTIL_TIME_H_
