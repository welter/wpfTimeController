
#define temp "CREATE TABLE DEPARTMENT(ID INT PRIMARY KEY NOT NULL,DEPT  CHAR(50) NOT NULL,EMP_ID  INT NOT NULL); "

#define SQL_CreateRuleTable "CREATE TABLE RULE ( ID INTEGER PRIMARY KEY autoincrement,RULENAME TEXT, TASKNAME TEXT, PROGRAMTITLE TEXT, PROGRAMDIRECTORY TEXT, RUNPATH TEXT, \
RUNNINGRULE INT, STARTTIMESEC INT,STARTTIMEUSEC INT, ENDTIMESEC INT, ENDTIMEUSEC INT, PERPERIODTIME INT, TIMES INT, TOTALTIMESEC INT\
, TOTALTIMEUSEC INT,LIMITRULE INT);"

#define SQL_DeleteRule(RULEID) "DELETE FROM RULE WHERE RULEID='"+RULEID+"'"

#define SQL_AddRule(RULE) "INSERT INTO RULE VALUES('NULL,"+to_string(RULE.GetId())+","+RULE.GetRuleName()+","+RULE.GetTaskName()+","+RULE.GetProgramTitle()+","+RULE.GetProgramDirectory()\
+","+RULE.GetRunPath()+","+to_string(static_cast<int>(RULE.GetRunningRule()))+","+to_string(RULE.GetStartTime().tv_sec)+","+to_string(RULE.GetStartTime().tv_usec)+","\
+to_string(RULE.GetEndTime().tv_sec)+","+to_string(RULE.GetEndTime().tv_usec)+","+to_string(RULE.GetPerPeriodTime())+","\
+to_string(RULE.GetTimes())+","+to_string(RULE.GetTotalTime().tv_sec)+","+to_string(RULE.GetTotalTime().tv_usec)+","+to_string(static_cast<int>(RULE.GetLimitRule()))+"'"

#define SQL_ClearRule "DELETE FROM RULE"

#define SQL_GetRuleByName "SELECT * FROM RULE WHERE RULENAME=:rulename"

#define SQL_GetRuleByID "SELECT * FROM RULE WHERE RULEID=:ruleid"

#define SQL_SetRuleByName "UPDATE RULE SET TASKNAME=:taskname,PROGRAMTITLE=:programtitle,PROGRAMDIRECTORY=:programdirectory,RUNPATH:=runpath,\
RUNNINGRULE=:runningrule,STARTTIMESEC=:starttimesec,STARTTIMEUSEC=:starttimeusec,ENDTIMESEC=:endtimesec,ENDTIMEUSEC=:endtimeusec,PERPERIODTIME=:perperiodtime\
,TIMES=:times,TOTALTIMESEC=:totaltimesec,TOTALTIMEUSEC=:totaltimeusec,LIMITRULE=:limitrule WHERE RULENAME=:rulename"

#define SQL_SetRuleByID "UPDATE RULE SET RULENAME=:rulename,TASKNAME=:taskname,PROGRAMTITLE=:programtitle,PROGRAMDIRECTORY=:programdirectory,RUNPATH:=runpath,\
RUNNINGRULE=:runningrule,STARTTIMESEC=:starttimesec,STARTTIMEUSEC=:starttimeusec,ENDTIMESEC=:endtimesec,ENDTIMEUSEC=:endtimeusec,PERPERIODTIME=:perperiodtime\
,TIMES=:times,TOTALTIMESEC=:totaltimesec,TOTALTIMEUSEC=:totaltimeusec,LIMITRULE=:limitrule WHERE RULENAME=:rulename"