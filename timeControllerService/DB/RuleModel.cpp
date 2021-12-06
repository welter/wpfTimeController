#include "RuleModel.h"
namespace DB {


    int TimeControllerRule::GetId() {
        return Id;
    }

    void TimeControllerRule::SetId(int id) {
        Id = id;
    }

    string TimeControllerRule::GetRuleName() {
        return RuleName;
    }

    void TimeControllerRule::SetRuleName(string ruleName) {
        RuleName = ruleName;
    }

    string TimeControllerRule::GetProgramName() {
        return ProgramName;
    }

    void TimeControllerRule::SetProgramName(string programName) {
        ProgramName = programName;
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

    unsigned char TimeControllerRule::GetRunMode() {
        return RunMode;
    }

    void TimeControllerRule::SetRunMode(unsigned char runMode) {
        RunMode = runMode;
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

    int TimeControllerRule::GetPerPeriodTime() {
        return PerPeriodTime;
    }

    void TimeControllerRule::SetPerPeriodTime(int perPeriodTime) {
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

    void TimeControllerRule::SetTotalTime(timeval totalTime) {
        TotalTime = totalTime;
    }

    DB::LimitRule TimeControllerRule::GetLimitRule() {
        return LimitRule;
    }

    void TimeControllerRule::SetLimitRule(DB::LimitRule limitRule) {
        LimitRule = limitRule;
    }
}