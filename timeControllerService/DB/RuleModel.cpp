namespace DB {
#include "RuleModel.h"

    long TimeControllerRule::GetId() {
        return Id;
    }

    void TimeControllerRule::SetId(long id) {
        Id = id;
    }

    string TimeControllerRule::GetTaskName() {
    }

    void TimeControllerRule::SetTaskName(string taskName) {
    }

    string TimeControllerRule::GetProgramTitle() {
    }

    void TimeControllerRule::SetProgramTitle(string programtitle) {
    }

    string TimeControllerRule::GetProgramDirectory() {
    }
    void TimeControllerRule::SetProgramDirectory(string programDirectory) {
    }

    string TimeControllerRule::GetRunPath() {
    }

    void TimeControllerRule::SetRunPath(string RunPath) {
    }

    DB::RunningRule TimeControllerRule::GetRunningRule() {
    }

    void TimeControllerRule::SetRunningRule(DB::RunningRule runningRule) {
    }

    timeval TimeControllerRule::GetStartTime() {
    }

    void TimeControllerRule::SetStartTime(timeval startTime) {
    }

    timeval TimeControllerRule::GetEndTime() {
    }

    void TimeControllerRule::SetEndTime(timeval endTime) {
    }

    timeval TimeControllerRule::GetPerPeriodTime() {
    }

    void TimeControllerRule::SetPerPeriodTime(timeval perPeriodTime) {
    }

    int TimeControllerRule::GetTimes() {
    }

    void TimeControllerRule::SetTimes(int times) {
    }

    timeval TimeControllerRule::GetTotalTime() {
    }

    void TimeControllerRule::SetGetTotalTime(timeval totalTime) {
    }

    DB::LimitRule TimeControllerRule::GetLimitRule() {
    }

    void TimeControllerRule::SetLimitRule(DB::LimitRule limitRule) {
    }
}