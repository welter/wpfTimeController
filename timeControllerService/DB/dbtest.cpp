#include <windows.h>
#include "DBRuleService.h"
#include "common.h"
#include "RuleModel.h"
int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow)
{
    DB::DBRuleService DBRS;
    DB::TimeControllerRule* rule=new DB::TimeControllerRule();
    /*rule->SetRuleName("1");
    rule->SetTaskName("2");
    rule->SetProgramTitle("3");
    rule->SetProgramDirectory("4");
    rule->SetRunPath("5");
    rule->SetStartTime(DB::timeval{  5, 10 });
    rule->SetEndTime(DB::timeval{ 10,20 });
    rule->SetPerPeriodTime(5);
    rule->SetTimes(6);
    rule->SetRunningRule(DB::RunningRule::a);
    rule->SetLimitRule(DB::LimitRule::f);
    rule->SetTotalTime(DB::timeval{ 30,40 });

    DBRS.addRule(rule);*/

    DBRS.getRule(rule,"1");
    
}