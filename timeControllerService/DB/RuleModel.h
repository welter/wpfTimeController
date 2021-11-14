#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include "common.h"

namespace DB {
    using namespace std;
    class TimeControllerRule {
    private:
        long Id;
        string TaskName;
        string ProgramTitle;
        string ProgramDirectory;
        string RunPath;
        RunningRule RunningRule;
        timeval StartTime;
        timeval EndTime;
        timeval PerPeriodTime;
        int Times;
        timeval TotalTime;
        LimitRule LimitRule;
    public:
        long GetId();
        void SetId(long id);
        string GetTaskName();
        void SetTaskName(string taskName);
        string GetProgramTitle();
        void SetProgramTitle(string programtitle);
        string GetProgramDirectory();
        void SetProgramDirectory(string programDirectory);
        string GetRunPath();
        void SetRunPath(string RunPath);
        DB::RunningRule GetRunningRule();
        void SetRunningRule(DB::RunningRule runningRule);
        timeval GetStartTime();
        void SetStartTime(timeval startTime);
        timeval GetEndTime();
        void SetEndTime(timeval endTime);
        timeval GetPerPeriodTime();
        void SetPerPeriodTime(timeval perPeriodTime);
        int GetTimes();
        void SetTimes(int times);
        timeval GetTotalTime();
        void SetGetTotalTime(timeval totalTime);
        DB::LimitRule GetLimitRule();
        void SetLimitRule(DB::LimitRule limitRule);
    };
}