#include <gtest/gtest.h>
#include "foundation/log/log.h"
#include "foundation/log/file_logger.h"
#include "foundation/log/stdout_logger.h"

TEST(stdlogger_stream_utest, print1) {
    std::shared_ptr<fdan::Logger> log = std::make_shared<fdan::StdoutLogger>();
    fdan::LOG_SET(log);
    fdan::LOG_SET_LEVEL(fdan::LL_ERROR);
    
    fdan::LOG_DEBUG_S << "it is a debug test log.";
    fdan::LOG_INFO_S << "it is a info test num";
    fdan::LOG_WARN_S <<  "it is a warn test num";
    fdan::LOG_ERROR_S <<  "it is a error test num";
    fdan::LOG_FATAL_S <<  "it is a fatal test num";
}

TEST(filelogger_stream_utest, debug) {
    std::shared_ptr<fdan::Logger> file_log = std::make_shared<fdan::FileLogger>("test_log.log");
    std::shared_ptr<fdan::Logger> std_log = std::make_shared<fdan::StdoutLogger>();
    file_log->SetLogger(std_log);
    fdan::LOG_SET(file_log);
    fdan::LOG_SET_LEVEL(fdan::LL_DEBUG);
    
    fdan::LOG_DEBUG_S << "it is a debug test log.";
    fdan::LOG_INFO_S << "it is a info test num";
    fdan::LOG_WARN_S <<  "it is a warn test num";
    fdan::LOG_ERROR_S <<  "it is a error test num";
    fdan::LOG_FATAL_S <<  "it is a fatal test num";
}

TEST(filelogger_stream_utest, value) {
    std::shared_ptr<fdan::Logger> file_log = std::make_shared<fdan::FileLogger>("test_log.log");
    std::shared_ptr<fdan::Logger> std_log = std::make_shared<fdan::StdoutLogger>();
    file_log->SetLogger(std_log);
    fdan::LOG_SET(file_log);
    fdan::LOG_SET_LEVEL(fdan::LL_DEBUG);
    
    fdan::LOG_FATAL_S <<  "bool value:" << true;
    fdan::LOG_FATAL_S <<  "int8 value:" << int8_t(32);
    fdan::LOG_FATAL_S <<  "uint8 value:" << uint8_t(-1);
    fdan::LOG_FATAL_S <<  "int16 value:" << int16_t(128);
    fdan::LOG_FATAL_S <<  "uint16 value:" << uint16_t(-1);
    fdan::LOG_FATAL_S <<  "int32 value:" << int32_t(6451);
    fdan::LOG_FATAL_S <<  "uint32 value:" << uint32_t(-1);
    fdan::LOG_FATAL_S <<  "int64 value:" << int64_t(-515811548);
    fdan::LOG_FATAL_S <<  "uint64 value:" << uint64_t(-1);
    fdan::LOG_FATAL_S <<  "long long value:" << (long long)(54561648451166);
    fdan::LOG_FATAL_S <<  "float value:" << float(1.1111111111111);
    fdan::LOG_FATAL_S <<  "double value:" << double(1.45516167894165);;
    fdan::LOG_FATAL_S <<  "string value:" << std::string("it is a test log");
    fdan::LOG_FATAL_S <<  "const char* value:" << "it is a test log";
    fdan::LOG_FATAL_S <<  "char value:" << 'N';
}
