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
    rule->SetRuleName("2");
    rule->SetTaskName("3");
    rule->SetProgramTitle("3");
    rule->SetProgramDirectory("4");
    rule->SetRunPath("5");
    rule->SetStartTime(DB::timeval{  15, 10 });
    rule->SetEndTime(DB::timeval{ 20,20 });
    rule->SetPerPeriodTime(15);
    rule->SetTimes(16);
    rule->SetRunningRule(DB::RunningRule::b);
    rule->SetLimitRule(DB::LimitRule::g);
    rule->SetTotalTime(DB::timeval{ 30,40 });

    /*DBRS.addRule(rule); */

    DBRS.setRule("2",rule);
    
}