#include "RuleModel.h"
namespace DB {


    int TimeControllerRule::GetId() {
        return Id;
    }

    void TimeControllerRule::SetId(int id) {
        Id = id;
    }

    string TimeControllerRule::GetTaskName() {
        return TaskName;
    }

    void TimeControllerRule::SetTaskName(string taskName) {
        TaskName = taskName;
    }

    string TimeControllerRule::GetProgramTitle() {
        return ProgramTitle;
    }

    void TimeControllerRule::SetProgramTitle(string programTitle) {
        ProgramTitle = programTitle;
    }

    string TimeControllerRule::GetProgramDirectory() {
        return ProgramDirectory;
    }

    void TimeControllerRule::SetProgramDirectory(string programDirectory) {
        ProgramDirectory = programDirectory;
    }

    string TimeControllerRule::GetRunPath() {
        return RunPath;
    }

    void TimeControllerRule::SetRunPath(string runPath) {
        RunPath = runPath;
    }

    DB::RunningRule TimeControllerRule::GetRunningRule() {
        return RunningRule;
    }

    void TimeControllerRule::SetRunningRule(DB::RunningRule runningRule) {
        RunningRule = runningRule;
    }

    timeval TimeControllerRule::GetStartTime() {
        return StartTime;
    }

    void TimeControllerRule::SetStartTime(timeval startTime) {
        StartTime = startTime;
    }

    timeval TimeControllerRule::GetEndTime() {
        return EndTime;
    }

    void TimeControllerRule::SetEndTime(timeval endTime) {
        EndTime = endTime;
    }

    timeval TimeControllerRule::GetPerPeriodTime() {
        return PerPeriodTime;
    }

    void TimeControllerRule::SetPerPeriodTime(timeval perPeriodTime) {
        PerPeriodTime = perPeriodTime;
    }

    int TimeControllerRule::GetTimes() {
        return Times;
    }

    void TimeControllerRule::SetTimes(int times) {
        Times = times;
    }

    timeval TimeControllerRule::GetTotalTime() {
        return TotalTime;
    }

    void TimeControllerRule::SetGetTotalTime(timeval totalTime) {
        TotalTime = totalTime;
    }

    DB::LimitRule TimeControllerRule::GetLimitRule() {
        return LimitRule;
    }

    void TimeControllerRule::SetLimitRule(DB::LimitRule limitRule) {
        LimitRule = limitRule;
    }
}