#include <string>
#include <gtest/gtest.h>
#include "../time_consuming.h"
#include "foundation/util/time.h"

TEST(time_utest, get_time1) {
    std::cout << fdan::UTCTimeSec() << std::endl;
    std::cout << fdan::UTCTimeMsec() << std::endl;
    std::cout << fdan::GetFormatTime() << std::endl;

    char buf[fdan::kFormatTimeBufSize] = {0};
    uint32_t size = fdan::kFormatTimeBufSize;
    fdan::GetFormatTime(buf, size);
    std::cout << buf << std::endl;
}


TEST(time_utest, get_time2) {
    std::string year = fdan::GetFormatTime(fdan::FTU_YEAR);
    EXPECT_EQ(year.length(), sizeof("xxxx") - 1);
    
    std::string month = fdan::GetFormatTime(fdan::FTU_MONTH);
    EXPECT_EQ(month.length(), sizeof("xxxx-xx") - 1);

    std::string day = fdan::GetFormatTime(fdan::FTU_DAY);
    EXPECT_EQ(day.length(), sizeof("xxxx-xx-xx") - 1);

    std::string hour = fdan::GetFormatTime(fdan::FTU_HOUR);
    EXPECT_EQ(hour.length(), sizeof("xxxx-xx-xx xx") - 1);

    std::string minute = fdan::GetFormatTime(fdan::FTU_MINUTE);
    EXPECT_EQ(minute.length(), sizeof("xxxx-xx-xx xx:xx") - 1);

    std::string second = fdan::GetFormatTime(fdan::FTU_SECOND);
    EXPECT_EQ(second.length(), sizeof("xxxx-xx-xx xx:xx:xx") - 1);

    std::string millisecond = fdan::GetFormatTime(fdan::FTU_MILLISECOND);
    EXPECT_EQ(millisecond.length(), sizeof("xxxx-xx-xx xx:xx:xx:xxx") - 1);
}