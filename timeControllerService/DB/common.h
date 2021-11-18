#pragma once
namespace DB {
	enum RunningRule {a,b,c,d,e};
	enum LimitRule {a,b,c,d,e};

#define enum_to_string(x) #x

    struct timeval
    {
        long tv_sec; /*√Î*/
        long tv_usec; /*Œ¢√Î*/
    };

}
