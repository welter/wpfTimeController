#include <windows.h>
#include "DBRuleService.h"
#include "common.h"
#include <conio.h>
#include <iostream>
#include "RuleModel.h"
#include "../Timer/Timer.h"

#define KEYDOWN( vk ) ( 0x8000 & ::GetAsyncKeyState( vk ) ) 

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
    rule->SetRunMode(DB::rmTimes);
    rule->SetLimitRule(DB::LimitRule::g);
    rule->SetTotalTime(DB::timeval{ 30,40 });
    WindowsTimer timer;
    timer.setCallback(callb);
    timer.start(1, false);
    while (true)
    {
        if (KEYDOWN(VK_ESCAPE)) // 按ESC退出,非阻塞模式，每次循环不会停留在这
            break;
        using namespace std;
        cout << "1" << endl;
    }
    return 0;

    /*DBRS.addRule(rule); */

    //DBRS.setRule("2",rule);
    
}