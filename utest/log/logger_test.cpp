#include <gtest/gtest.h>
#include "foundation/log/log.h"
#include "foundation/log/file_logger.h"
#include "foundation/log/stdout_logger.h"

TEST(stdlogger_utest, print1) {
    std::shared_ptr<fdan::Logger> log = std::make_shared<fdan::StdoutLogger>();
    fdan::LOG_SET(log);
    fdan::LOG_SET_LEVEL(fdan::LL_ERROR);
    
    fdan::LOG_DEBUG("it is a debug test log.");
    fdan::LOG_INFO("%s %d", "it is a info test num", 100191);
    fdan::LOG_WARN("%s %d", "it is a warn test num", 100191);
    fdan::LOG_ERROR("%s %d", "it is a error test num", 100191);
    fdan::LOG_FATAL("%s %d", "it is a fatal test num", 100191);
}


TEST(filelogger_utest, debug) {
    std::shared_ptr<fdan::Logger> file_log = std::make_shared<fdan::FileLogger>("test_log.log");
    std::shared_ptr<fdan::Logger> std_log = std::make_shared<fdan::StdoutLogger>();
    file_log->SetLogger(std_log);
    fdan::LOG_SET(file_log);
    fdan::LOG_SET_LEVEL(fdan::LL_DEBUG);
    
    fdan::LOG_DEBUG("it is a debug test log.");
    fdan::LOG_INFO("%s %d", "it is a info test num", 100191);
    fdan::LOG_WARN("%s %d", "it is a warn test num", 100191);
    fdan::LOG_ERROR("%s %d", "it is a error test num", 100191);
    fdan::LOG_FATAL("%s %d", "it is a fatal test num", 100191);
}