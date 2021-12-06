#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include "common.h"

namespace DB {
    using namespace std;
    class TimeControllerRule {
    private:
        int Id;
        string RuleName;
        string ProgramName;
        string ProgramTitle;
        string ProgramDirectory;
        string RunPath;
        unsigned char RunMode;
        timeval StartTime;
        timeval EndTime;
        int PerPeriodTime;
        int Times;
        timeval TotalTime;
        LimitRule LimitRule;
    public:
        int GetId();
        void SetId(int id);
        string GetRuleName();
        void SetRuleName(string ruleName);
        string GetProgramName();
        void SetProgramName(string programName);
        string GetProgramTitle();
        void SetProgramTitle(string programTitle);
        string GetProgramDirectory();
        void SetProgramDirectory(string programDirectory);
        string GetRunPath();
        void SetRunPath(string runPath);
        unsigned char GetRunMode();
        void SetRunMode(unsigned char runMode);
        timeval GetStartTime();
        void SetStartTime(timeval startTime);
        timeval GetEndTime();
        void SetEndTime(timeval endTime);
        int GetPerPeriodTime();
        void SetPerPeriodTime(int perPeriodTime);
        int GetTimes();
        void SetTimes(int times);
        timeval GetTotalTime();
        void SetTotalTime(timeval totalTime);
        DB::LimitRule GetLimitRule();
        void SetLimitRule(DB::LimitRule limitRule);
    };
}