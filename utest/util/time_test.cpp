#include <string>
#include <gtest/gtest.h>
#include "common/util/time.h"
#include "../time_consuming.h"

TEST(time_utest, get_time1) {
    std::cout << quicx::UTCTimeSec() << std::endl;
    std::cout << quicx::UTCTimeMsec() << std::endl;
    std::cout << quicx::GetFormatTime() << std::endl;

    char buf[quicx::__format_time_buf_size] = {0};
    uint32_t size = quicx::__format_time_buf_size;
    quicx::GetFormatTime(buf, size);
    std::cout << buf << std::endl;
}


TEST(time_utest, get_time2) {
    std::string year = quicx::GetFormatTime(quicx::FTU_YEAR);
    EXPECT_EQ(year.length(), sizeof("xxxx") - 1);
    
    std::string month = quicx::GetFormatTime(quicx::FTU_MONTH);
    EXPECT_EQ(month.length(), sizeof("xxxx-xx") - 1);

    std::string day = quicx::GetFormatTime(quicx::FTU_DAY);
    EXPECT_EQ(day.length(), sizeof("xxxx-xx-xx") - 1);

    std::string hour = quicx::GetFormatTime(quicx::FTU_HOUR);
    EXPECT_EQ(hour.length(), sizeof("xxxx-xx-xx xx") - 1);

    std::string minute = quicx::GetFormatTime(quicx::FTU_MINUTE);
    EXPECT_EQ(minute.length(), sizeof("xxxx-xx-xx xx:xx") - 1);

    std::string second = quicx::GetFormatTime(quicx::FTU_SECOND);
    EXPECT_EQ(second.length(), sizeof("xxxx-xx-xx xx:xx:xx") - 1);

    std::string millisecond = quicx::GetFormatTime(quicx::FTU_MILLISECOND);
    EXPECT_EQ(millisecond.length(), sizeof("xxxx-xx-xx xx:xx:xx:xxx") - 1);
}