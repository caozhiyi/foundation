#include <gtest/gtest.h>
#include "common/log/log.h"
#include "common/log/file_logger.h"
#include "common/log/stdout_logger.h"

TEST(stdlogger_utest, print1) {
    std::shared_ptr<quicx::Logger> log = std::make_shared<quicx::StdoutLogger>();
    quicx::LOG_SET(log);
    quicx::LOG_SET_LEVEL(quicx::LL_ERROR);
    
    quicx::LOG_DEBUG("it is a debug test log.");
    quicx::LOG_INFO("%s %d", "it is a info test num", 100191);
    quicx::LOG_WARN("%s %d", "it is a warn test num", 100191);
    quicx::LOG_ERROR("%s %d", "it is a error test num", 100191);
    quicx::LOG_FATAL("%s %d", "it is a fatal test num", 100191);
}


TEST(filelogger_utest, debug) {
    std::shared_ptr<quicx::Logger> file_log = std::make_shared<quicx::FileLogger>();
    std::shared_ptr<quicx::Logger> std_log = std::make_shared<quicx::StdoutLogger>();
    file_log->SetLogger(std_log);
    quicx::LOG_SET(file_log);
    quicx::LOG_SET_LEVEL(quicx::LL_DEBUG);
    
    quicx::LOG_DEBUG("it is a debug test log.");
    quicx::LOG_INFO("%s %d", "it is a info test num", 100191);
    quicx::LOG_WARN("%s %d", "it is a warn test num", 100191);
    quicx::LOG_ERROR("%s %d", "it is a error test num", 100191);
    quicx::LOG_FATAL("%s %d", "it is a fatal test num", 100191);
}