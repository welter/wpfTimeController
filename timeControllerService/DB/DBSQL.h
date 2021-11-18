
#define SQL_CreateRuleTable "CREATE TABLE RULE ( ID nteger PRIMARY KEY autoincrement, TASKNAME TEXT, PROGRAMTITLE TEXT, PROGRAMDIRECTORY TEXT, RUNPATH TEXT, \
RUNNINGRULE INT, STARTTIMESEC INT,STARTTIMEUSEC INT, ENDTIMESEC INT ENDTIMEUSEC INT, PERPERIODTIMESEC INT, PERPERIODTIMEUSEC INT, TIMES INT, TOTALTIMESEC INT\
, TOTALTIMEUSEC INT LIMITRULE INT )"

#define SQL_DeleteRule(RULEID) "DELETE FROM RULE WHERE RULEID='"+RULEID+"'"

#define SQL_AddRule(RULE) "INSERT INTO RULE VALUES('NULL,"+to_string(RULE.GetId())+","+RULE.GetTaskName()+","+RULE.GetProgramTitle()+","+RULE.GetProgramDirectory()\
+","+RULE.GetRunPath()+","+to_string(static_cast<int>(RULE.GetRunningRule()))+","+to_string(RULE.GetStartTime().tv_sec)+","+to_string(RULE.GetStartTime().tv_usec)+","\
+to_string(RULE.GetEndTime().tv_sec)+","+to_string(RULE.GetEndTime().tv_usec)+","+to_string(RULE.GetPerPeriodTime().tv_sec)+","+to_string(RULE.GetPerPeriodTime().tv_usec)+"'"\
+to_string(RULE.GetTimes())+","+to_string(RULE.GetTotalTime().tv_sec)+","+to_string(RULE.GetTotalTime().tv_usec)+","+to_string(static_cast<int>(RULE.GetLimitRule()))+"'"

#define SQL_ClearRule "DELETE FROM RULE"