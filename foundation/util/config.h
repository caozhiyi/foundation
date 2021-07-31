// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)
// Copyright <caozhiyi5@gmail.com>

#ifndef FOUNDATION_UTIL_CONFIG_H_
#define FOUNDATION_UTIL_CONFIG_H_

#include <map>
#include <mutex>
#include <string>

namespace fdan {

class Config {
 public:
  void SetFilePath(const std::string& path);
  bool ReLoadFile();

  bool LoadFile(const std::string& path);

  int GetIntValue(const std::string& key);
  std::string GetStringValue(const std::string& key);
  double GetDoubleValue(const std::string& key);
  bool GetBoolValue(const std::string& key);

 private:
  void _Trim(std::string& line);

 private:
  std::string file_;
  std::mutex  mutex_;
  std::map<std::string, std::string> config_map_;
};

}  // namespace fdan

#endif  // FOUNDATION_UTIL_CONFIG_H_
