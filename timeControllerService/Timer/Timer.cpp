#include "Timer.h"

#include <winbase.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <cassert>


namespace {
    // impl. constants
    const int c24Hours = 60 * 60 * 24; // in seconds
    const int timeScale = 1; 
    // impl helper
    std::string safeCtime(std::time_t* time)
    {
        char buffer[40];
        errno_t err = ctime_s(buffer, 40, time);
        assert(!err);
        return buffer;
    }
}


WindowsTimer::WindowsTimer(CallbackFn callb)
    : _callbackFn(callb),
    _timerHandle(NULL)
{
}


WindowsTimer::~WindowsTimer()
{
    stop();
}


void WindowsTimer::setCallback(CallbackFn callb)
{
    assert(callb != nullptr);
    _callbackFn = callb;
}


bool WindowsTimer::start(int interval, bool cyclic)
{
    if (!_callbackFn) {
        assert(false);
        return false;
    }

#if _DEBUG
    std::time_t now = std::time(nullptr);
    now += interval;
    std::cout << "INFO: first timeout - " << safeCtime(&now);
#endif

    return start(interval, cyclic ? interval : 0);
}


bool WindowsTimer::start(const std::string& daytimeStrg, bool cyclic)
{
    if (!_callbackFn) {
        assert(false);
        return false;
    }

    int hour = -1;
    int min = -1;

    if (checkHourStrgFormat(daytimeStrg, &hour, &min) != FormatOk) {
        std::cout << "ERROR:: invalid time string passed: " << daytimeStrg << std::endl;
        return false;
    }

    std::time_t now = std::time(nullptr);
    std::tm restartTime;
    errno_t err = localtime_s(&restartTime, &now);
    assert(!err);

    int currentHour = restartTime.tm_hour;
    int currentMin = restartTime.tm_min;

    restartTime.tm_hour = hour;
    restartTime.tm_min = min;
    restartTime.tm_sec = 0;

    std::time_t then = std::mktime(&restartTime);

    if (hour < currentHour || (hour == currentHour && min <= currentMin)) {
        then += c24Hours; // next day!
    }

    double seconds = difftime(then, now);
    int timeFromNow = static_cast<int>(std::floor(seconds));

#if _DEBUG
    std::cout << "INFO: first timeout - " << safeCtime(&then);
    std::cout << "INFO: first timeout - timeFromNow: " << timeFromNow << " [sec]" << std::endl;

    if (cyclic) {
        std::time_t then2 = then;
        then2 += c24Hours;
        std::cout << "INFO: next timeout - " << safeCtime(&then2);
    }
#endif

    return start(timeFromNow, cyclic ? c24Hours : 0);
}


bool WindowsTimer::stop()
{
    if (_timerHandle == NULL) {
        return true;
    }

    if (!DeleteTimerQueueTimer(NULL, _timerHandle, INVALID_HANDLE_VALUE)) {
        int code = GetLastError();
        std::cout << "ERROR:: DeleteTimerQueueTimer failed, errCode=" << code << std::endl;
        return false;
    }
    else {
        _timerHandle = NULL;
        return true;
    }
}


WindowsTimer::HourStringCheckStatus
WindowsTimer::checkHourStrgFormat(const std::string& timeStrg, int* hour, int* min)
{
    // OPEN TODO::: use a std::regex when porting to C++11!

    bool formatOk =
        timeStrg.size() == 5 &&
        isdigit(timeStrg[0]) && isdigit(timeStrg[1]) &&
        timeStrg[2] == ':' &&
        isdigit(timeStrg[3]) && isdigit(timeStrg[4]);

    if (!formatOk) {
        return BadFormat;
    }

    std::string hours = timeStrg.substr(0, 2);
    std::string mins = timeStrg.substr(3, 2);

    formatOk =
        atoi(hours.c_str()) <= 24 &&
        atoi(mins.c_str()) <= 59;

    if (!formatOk) {
        return BadValue;
    }

    // ready
    if (hour) {
        *hour = atoi(hours.c_str());
    }
    if (min) {
        *min = atoi(mins.c_str());
    }

    return FormatOk;
}


std::string WindowsTimer::currTimeStrg(bool seconds)
{
    std::time_t rawtime = std::time(nullptr);
    std::tm timeinfo;

    errno_t err = localtime_s(&timeinfo, &rawtime);
    assert(!err);

    char buffer[10];

    if (!seconds) {
        strftime(buffer, 10, "%H:%M", &timeinfo);
    }
    else {
        strftime(buffer, 10, "%H:%M:%S", &timeinfo);
    }

    return buffer;
}


// private methods

bool WindowsTimer::start(int dueTime, int period)
{
    if (_timerHandle != NULL) {
        stop();
    }
    int flags = 0;

    if (!CreateTimerQueueTimer(&_timerHandle, NULL, callbackWrapper, _callbackFn,
        dueTime * timeScale, period * timeScale,
        flags)) {
        int code = GetLastError();
        std::cout << "ERROR:: CreateTimerQueueTimer failed, errCode=" << code << std::endl;

        return false;
    }

    return true;
}


VOID CALLBACK WindowsTimer::callbackWrapper(PVOID parameter, BOOLEAN fired)
{
#if _DEBUG
    assert(parameter != NULL);
    std::cout << "INFO: in WindowsTimer callback..." << std::endl;
#endif

    CallbackFn callb = reinterpret_cast<CallbackFn>(parameter);
    callb();
}