#include <gtest/gtest.h>
#include "common/log/log.h"
#include "common/log/file_logger.h"
#include "common/log/stdout_logger.h"

TEST(stdlogger_stream_utest, print1) {
    std::shared_ptr<quicx::Logger> log = std::make_shared<quicx::StdoutLogger>();
    quicx::LOG_SET(log);
    quicx::LOG_SET_LEVEL(quicx::LL_ERROR);
    
    quicx::LOG_DEBUG_S << "it is a debug test log.";
    quicx::LOG_INFO_S << "it is a info test num";
    quicx::LOG_WARN_S <<  "it is a warn test num";
    quicx::LOG_ERROR_S <<  "it is a error test num";
    quicx::LOG_FATAL_S <<  "it is a fatal test num";
}

TEST(filelogger_stream_utest, debug) {
    std::shared_ptr<quicx::Logger> file_log = std::make_shared<quicx::FileLogger>();
    std::shared_ptr<quicx::Logger> std_log = std::make_shared<quicx::StdoutLogger>();
    file_log->SetLogger(std_log);
    quicx::LOG_SET(file_log);
    quicx::LOG_SET_LEVEL(quicx::LL_DEBUG);
    
    quicx::LOG_DEBUG_S << "it is a debug test log.";
    quicx::LOG_INFO_S << "it is a info test num";
    quicx::LOG_WARN_S <<  "it is a warn test num";
    quicx::LOG_ERROR_S <<  "it is a error test num";
    quicx::LOG_FATAL_S <<  "it is a fatal test num";
}

TEST(filelogger_stream_utest, value) {
    std::shared_ptr<quicx::Logger> file_log = std::make_shared<quicx::FileLogger>();
    std::shared_ptr<quicx::Logger> std_log = std::make_shared<quicx::StdoutLogger>();
    file_log->SetLogger(std_log);
    quicx::LOG_SET(file_log);
    quicx::LOG_SET_LEVEL(quicx::LL_DEBUG);
    
    quicx::LOG_FATAL_S <<  "bool value:" << true;
    quicx::LOG_FATAL_S <<  "int8 value:" << int8_t(32);
    quicx::LOG_FATAL_S <<  "uint8 value:" << uint8_t(-1);
    quicx::LOG_FATAL_S <<  "int16 value:" << int16_t(128);
    quicx::LOG_FATAL_S <<  "uint16 value:" << uint16_t(-1);
    quicx::LOG_FATAL_S <<  "int32 value:" << int32_t(6451);
    quicx::LOG_FATAL_S <<  "uint32 value:" << uint32_t(-1);
    quicx::LOG_FATAL_S <<  "int64 value:" << int64_t(-515811548);
    quicx::LOG_FATAL_S <<  "uint64 value:" << uint64_t(-1);
    quicx::LOG_FATAL_S <<  "long long value:" << (long long)(54561648451166);
    quicx::LOG_FATAL_S <<  "float value:" << float(1.1111111111111);
    quicx::LOG_FATAL_S <<  "double value:" << double(1.45516167894165);;
    quicx::LOG_FATAL_S <<  "string value:" << std::string("it is a test log");
    quicx::LOG_FATAL_S <<  "const char* value:" << "it is a test log";
    quicx::LOG_FATAL_S <<  "char value:" << 'N';
}
