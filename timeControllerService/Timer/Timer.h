#pragma once

#include <windows.h>
#include <string>


/**
@brief Basic simple timer implementation using only native Windows API. Use it
       on Windows if you don't have/want Qt or Boost in your project.
It can invoke only simple void-void callback functions, thus can be used to e.g.
set a global flag and return. It will fire at a given hour and minute or after
a given number of seconds. The exact time is given using a string in 24-hour
format, e.g. "21:17". You can run the timer in a *cyclic* manner, either to be
invoken every N seconds or every day at a given hour.

The callback will be run in a separate thread, so take care of synchronization!
Implementation uses C++89 standard for portability.
@author mrkkrj, ib-krajewski.de, 2019
*/
class WindowsTimer
{
public:
    typedef void(*CallbackFn)(void);

    WindowsTimer(CallbackFn callb = nullptr);
    ~WindowsTimer();

    void setCallback(CallbackFn callb);

    bool start(int interval, bool cyclic);
    bool start(const std::string& daytimeStrg, bool cyclic);
    bool stop();

    // time utilities
    enum HourStringCheckStatus {
        FormatOk, BadFormat, BadValue
    };
    static HourStringCheckStatus
        checkHourStrgFormat(const std::string& timeStrg,
            int* hour = nullptr, int* min = nullptr);
    static std::string currTimeStrg(bool seconds = false);
    static std::string currTimeStrgSeconds() { return currTimeStrg(true); }

private:
    bool start(int dueTime, int period);
    static VOID CALLBACK callbackWrapper(PVOID parameter, BOOLEAN fired);

    HANDLE _timerHandle;
    CallbackFn _callbackFn;
};