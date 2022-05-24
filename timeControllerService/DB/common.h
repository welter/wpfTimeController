#pragma once
#include <cstdint>
const unsigned char WM_TIMERCONTROLER_STOP = 1;
namespace DB {
    const unsigned char rmSpecifiedTime = 0b00000001;//在指定的时间段运行
    const unsigned char rmTimes = 0b00000010;    //每天运行的次数
    const unsigned char rmTimeinterval = 0b00000100;//每两次运行必须间隔的时间
    const unsigned char rmDuration = 0b00001000;//每次运行最长持续时间
    const unsigned char rmProhibitSpecifiedTime = 0b00010000;//禁止在指定的时间段运行
    const unsigned char rmTotalTime = 0b00100000;//每天总共运行时长
	enum LimitRule {f,g,h,i,j};

#define enum_to_string(x) #x

    struct timeval
    {
        long tv_sec; /*秒*/
        long tv_usec; /*微秒*/
    };
    class TimeControllerRule;

    

}
