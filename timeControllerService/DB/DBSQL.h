
#define temp "CREATE TABLE DEPARTMENT(ID INT PRIMARY KEY NOT NULL,DEPT  CHAR(50) NOT NULL,EMP_ID  INT NOT NULL); "

#define SQL_CreateRuleTable "CREATE TABLE RULE ( ID INTEGER PRIMARY KEY autoincrement,RULENAME TEXT, PROGRAMNAME TEXT, PROGRAMTITLE TEXT, PROGRAMDIRECTORY TEXT, RUNPATH TEXT, \
RUNMODE INT, STARTTIME INTEGER, ENDTIME INTEGER,  PERPERIODTIME INT, TIMES INT, TOTALTIME INTEGER\
, LIMITRULE INT);"

#define SQL_DeleteRule(RULEID) "DELETE FROM RULE WHERE RULEID='"+RULEID+"'"

#define SQL_DeleteRuleByName(RULENAME) "DELETE FROM RULE WHERE RULENAME='"+RULENAME+"'"

#define SQL_AddRule(RULE)  "INSERT INTO RULE (ID,RULENAME,PROGRAMNAME,PROGRAMTITLE,PROGRAMDIRECTORY,RUNPATH,RUNMODE,STARTTIME,STARTTIME,ENDTIME,ENDTIME,PERPERIODTIME,TIMES,TOTALTIME,TOTALTIME,LIMITRULE) VALUES (NULL,'"\
+RULE.GetRuleName()+"','"+RULE.GetProgramName()+"','"+RULE.GetProgramTitle()+"','"+RULE.GetProgramDirectory()\
+"','"+RULE.GetRunPath()+"','"+to_string(RULE.GetRunMode())+"','"+to_string(RULE.GetStartTime())+"','"\
+to_string(RULE.GetEndTime())+"','"+to_string(RULE.GetPerPeriodTime())+"','"\
+to_string(RULE.GetTimes())+"','"+to_string(RULE.GetTotalTime())+"','"+to_string(static_cast<int>(RULE.GetLimitRule()))+"')"

#define tempSQL "INSERT INTO RULE VALUES('NULL,RULENAME='"+RULE.GetRuleName()+"',PROGRAMNAME='"+RULE.GetProgramName()+"',PROGRAMTILE='"+RULE.GetProgramTitle()+"',PROGRAMDIRECTORY='"+RULE.GetProgramDirectory()\
+"',RUNPATH='"+RULE.GetRunPath()+"',RUNMode='"+to_string(RULE.GetRunMode())+"',STARTTIMESEC='"+to_string(RULE.GetStartTime().tv_sec)+"',STARTTIMEUSEC='"+to_string(RULE.GetStartTime().tv_usec)+"',ENDTIMESEC='"\
+to_string(RULE.GetEndTime().tv_sec)+"',ENDTIMEUSEC='"+to_string(RULE.GetEndTime().tv_usec)+"',PERPERIODTIME='"+to_string(RULE.GetPerPeriodTime())+"',TIMES='"\
+to_string(RULE.GetTimes())+"',TOTALTIMESEC='"+to_string(RULE.GetTotalTime().tv_sec)+"',TOTALTIMEUSEC='"+to_string(RULE.GetTotalTime().tv_usec)+"',LIMITRULE='"+to_string(static_cast<int>(RULE.GetLimitRule()))+"')"

#define SQL_ClearRule "DELETE FROM RULE"

#define SQL_GetRuleByName "SELECT * FROM RULE WHERE RULENAME=:rulename"

#define SQL_GetRuleByID "SELECT * FROM RULE WHERE ID=:ruleid"

#define SQL_SetRuleByName "UPDATE RULE SET PROGRAMNAME=:programname,PROGRAMTITLE=:programtitle,PROGRAMDIRECTORY=:programdirectory,RUNPATH=:runpath,\
RUNMODE=:runmode,STARTTIME=:starttime,ENDTIME=:endtime,PERPERIODTIME=:perperiodtime\
,TIMES=:times,TOTALTIME=:totaltime,LIMITRULE=:limitrule WHERE RULENAME=:rulename"

#define SQL_SetRuleByID "UPDATE RULE SET RULENAME=:rulename,PROGRAMNAME=:programname,PROGRAMTITLE=:programtitle,PROGRAMDIRECTORY=:programdirectory,RUNPATH=:runpath,\
RUNMODE=:runmode,STARTTIME=:starttime,ENDTIME=:endtime,PERPERIODTIME=:perperiodtime\
,TIMES=:times,TOTALTIME=:totaltime,LIMITRULE=:limitrule WHERE ID=:ruleid"

#define SQL_GetRuleCount "SELECT COUNT(*)  FROM RULE"
