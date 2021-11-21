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
        string TaskName;
        string ProgramTitle;
        string ProgramDirectory;
        string RunPath;
        RunningRule RunningRule;
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
        string GetTaskName();
        void SetTaskName(string taskName);
        string GetProgramTitle();
        void SetProgramTitle(string programTitle);
        string GetProgramDirectory();
        void SetProgramDirectory(string programDirectory);
        string GetRunPath();
        void SetRunPath(string runPath);
        DB::RunningRule GetRunningRule();
        void SetRunningRule(DB::RunningRule runningRule);
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