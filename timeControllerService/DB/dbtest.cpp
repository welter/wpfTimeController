#include <windows.h>
#include "DBRuleService.h"
#include "common.h"
#include "RuleModel.h"
#include "../Timer/Timer.h"
void callb() {
    std::cout << "test ok"<<std::endl;
    };
int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow)
{
    DB::DBRuleService DBRS;
    DB::TimeControllerRule* rule=new DB::TimeControllerRule();
    rule->SetRuleName("2");
    rule->SetProgramName("3");
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
    WindowsTimer timer;
    timer.setCallback(callb);
    timer.start(1, false);
    char* p="";
    while (*p!=14) {
        std::cin >> p;

    };
    /*DBRS.addRule(rule); */

    //DBRS.setRule("2",rule);
    
}