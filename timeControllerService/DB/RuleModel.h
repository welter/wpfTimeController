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
        time_t StartTime;
        time_t EndTime;
        int PerPeriodTime;
        int Times;
        time_t TotalTime;
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
        time_t GetStartTime();
        void SetStartTime(time_t startTime);
        time_t GetEndTime();
        void SetEndTime(time_t endTime);
        int GetPerPeriodTime();
        void SetPerPeriodTime(int perPeriodTime);
        int GetTimes();
        void SetTimes(int times);
        time_t GetTotalTime();
        void SetTotalTime(time_t totalTime);
        DB::LimitRule GetLimitRule();
        void SetLimitRule(DB::LimitRule limitRule);
    };
}