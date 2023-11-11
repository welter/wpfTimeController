#include "RuleService.h"
#include "RuleModel.h"
#include "DBSQL.h"
#include "common.h"
#include <array>
#include <vector>

namespace DB {
	using namespace std;
	const char* serviceDBPath = "rule.db";
	string s;
	int rc;
	SQLite::Database db = NULL;
	bool RuleService::closeTable() 
	{
		try {
			db.~Database();
		}
		catch (std::exception& e) {
			return false;
		}
		return true;
	}
	bool RuleService::openTable() 
	{

		
		try {
			db = std::move(SQLite::Database(serviceDBPath, SQLite::OPEN_READWRITE));
		}
		catch (std::exception& e)
		{
			//ConsolePrintf("exception: %s\n", e.what()); ConsoleScanf(ch, len);
			try {
				db = std::move(SQLite::Database(serviceDBPath, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE));
			}
			catch (std::exception& e) {
				return false;
			}
		}

		if (!db.tableExists("RULE")) {
			const char* sql;
			s = SQL_CreateRuleTable;
			sql = s.c_str();
			db.exec(sql);
		}
		return true;
	}


	int RuleService::addRule(TimeControllerRule* rule) {
		const char* sql;
		string r;
		openTable();
		r = SQL_AddRule((*rule));
		sql = r.c_str();
		rc = db.exec(sql);
		closeTable();
		return rc;
	}
	bool RuleService::deleteRule(int ruleID) {
		const char* sql;
		string r;
		openTable();
		r = SQL_DeleteRule(to_string(ruleID));
		sql = r.c_str();
		rc = db.exec(sql);
		closeTable();
		return rc;
	}
	bool RuleService::deleteRule(const char* ruleName) {
		const char* sql;
		string r;
		openTable();
		r = SQL_DeleteRuleByName((string)ruleName);
		sql = r.c_str();
		rc = db.exec(sql);
		closeTable();
		return rc;
	}
	bool RuleService::clear() {
		const char* sql;
		string r;
		openTable();
		r = SQL_ClearRule;
		sql = r.c_str();
		rc = db.exec(sql);
		closeTable();
		return rc;
	}
	bool RuleService::getRule(DB::TimeControllerRule* rule, const char* ruleName) {
		const char* sql;
		sql = SQL_GetRuleByName;
		openTable();
		SQLite::Statement mQuery(db, sql);
		mQuery.bind(":rulename", ruleName);
		//vector<TimeControllerRule*> a=vector<TimeControllerRule*>();       
		if (mQuery.executeStep())
		{
			//DB::TimeControllerRule* r=new DB::TimeControllerRule; 

			rule->SetId(mQuery.getColumn(0));
			rule->SetRuleName(mQuery.getColumn(1));
			rule->SetProgramName(mQuery.getColumn(2));
			rule->SetProgramTitle(mQuery.getColumn(3));
			rule->SetProgramDirectory(mQuery.getColumn(4));
			rule->SetRunPath(mQuery.getColumn(5));
			rule->SetRunMode(mQuery.getColumn(6).getInt());
			//struct timeval t;
			//t.tv_sec = mQuery.getColumn(7).getInt64();
			//t.tv_usec = mQuery.getColumn(8).getInt64();
			rule->SetStartTime(mQuery.getColumn(7).getInt64());
			//t.tv_sec = mQuery.getColumn(9).getInt64();
			//t.tv_usec = mQuery.getColumn(10).getInt64();
			rule->SetEndTime(mQuery.getColumn(8).getInt64());
			rule->SetPerPeriodTime(mQuery.getColumn(9));
			rule->SetTimes(mQuery.getColumn(10));
			//t.tv_sec = mQuery.getColumn(13).getInt64();
			//t.tv_usec = mQuery.getColumn(14).getInt64();
			rule->SetTotalTime(mQuery.getColumn(11).getInt64());
			rule->SetLimitRule((DB::LimitRule)mQuery.getColumn(12).getInt());
			//rule=&r;
			rule->SetInterval(mQuery.getColumn(13).getInt64());
		}
		closeTable();
		return true;
		//rule = &a;
	}
	bool RuleService::getRule(DB::TimeControllerRule* rule, int ruleID) {
		const char* sql;
		sql = SQL_GetRuleByID;
		openTable();
		SQLite::Statement mQuery(db, sql);
		mQuery.bind(":ruleid", ruleID);
		//vector<TimeControllerRule*> a=vector<TimeControllerRule*>();       
		if (mQuery.executeStep())
		{
			//DB::TimeControllerRule* r=new DB::TimeControllerRule; 

			rule->SetId(mQuery.getColumn(0));
			rule->SetRuleName(mQuery.getColumn(1));
			rule->SetProgramName(mQuery.getColumn(2));
			rule->SetProgramTitle(mQuery.getColumn(3));
			rule->SetProgramDirectory(mQuery.getColumn(4));
			rule->SetRunPath(mQuery.getColumn(5));
			rule->SetRunMode(mQuery.getColumn(6).getInt());
			//struct timeval t;
			//t.tv_sec = mQuery.getColumn(7).getInt64();
			//t.tv_usec = mQuery.getColumn(8).getInt64();
			rule->SetStartTime(mQuery.getColumn(7).getInt64());
			//t.tv_sec = mQuery.getColumn(9).getInt64();
			//t.tv_usec = mQuery.getColumn(10).getInt64();
			rule->SetEndTime(mQuery.getColumn(8).getInt64());
			rule->SetPerPeriodTime(mQuery.getColumn(9));
			rule->SetTimes(mQuery.getColumn(10));
			//t.tv_sec = mQuery.getColumn(13).getInt64();
			//t.tv_usec = mQuery.getColumn(14).getInt64();
			rule->SetTotalTime(mQuery.getColumn(11).getInt64());
			rule->SetLimitRule((DB::LimitRule)mQuery.getColumn(12).getInt());
			//rule=&r;
			rule->SetInterval(mQuery.getColumn(13).getInt64());
		}
		closeTable();
		return true;
		//rule = &a;
	}

	bool RuleService::setRule(const char* ruleName, DB::TimeControllerRule* rule) {
		const char* sql;
		sql = SQL_SetRuleByName;
		openTable();
		SQLite::Statement mQuery(db, sql);
		mQuery.bind(":programname", rule->GetProgramName());
		mQuery.bind(":programtitle", rule->GetProgramTitle());
		mQuery.bind(":programdirectory", rule->GetProgramDirectory());
		mQuery.bind(":runpath", rule->GetRunPath());
		mQuery.bind(":runmode", rule->GetRunMode());
		//struct timeval t;
		//t = rule->GetStartTime();
		mQuery.bind(":starttime", rule->GetStartTime());
		//mQuery.bind(":starttimeusec", t.tv_usec);
		//t = rule->GetEndTime();
		mQuery.bind(":endtime", rule->GetEndTime());
		//mQuery.bind(":endtimeusec", t.tv_usec);
		mQuery.bind(":perperiodtime", rule->GetPerPeriodTime());
		mQuery.bind(":times", rule->GetTimes());
		//t = rule->GetTotalTime();
		mQuery.bind(":totaltime", rule->GetTotalTime());
		//mQuery.bind(":totaltimeusec", t.tv_usec);
		mQuery.bind(":limitrule", rule->GetLimitRule());
		mQuery.bind(":interval", rule->GetInterval());
		mQuery.bind(":rulename", ruleName);
		mQuery.exec();
		closeTable();
		return true;
	}
	bool RuleService::setRule(int ruleID, DB::TimeControllerRule* rule) {
		const char* sql;
		sql = SQL_SetRuleByID;
		openTable();
		SQLite::Statement mQuery(db, sql);
		mQuery.bind(":programname", rule->GetProgramName());
		mQuery.bind(":programtitle", rule->GetProgramTitle());
		mQuery.bind(":programdirectory", rule->GetProgramDirectory());
		mQuery.bind(":runpath", rule->GetRunPath());
		mQuery.bind(":runmode", rule->GetRunMode());
		struct timeval t;
		//t = rule->GetStartTime();
		mQuery.bind(":starttime", rule->GetStartTime());
		//mQuery.bind(":starttimeusec", t.tv_usec);
		//t = rule->GetEndTime();
		mQuery.bind(":endtime", rule->GetEndTime());
		//mQuery.bind(":endtimeusec", t.tv_usec);
		mQuery.bind(":perperiodtime", rule->GetPerPeriodTime());
		mQuery.bind(":times", rule->GetTimes());
		//t = rule->GetTotalTime();
		mQuery.bind(":totaltime", rule->GetTotalTime());
		//mQuery.bind(":totaltimeusec", t.tv_usec);
		mQuery.bind(":limitrule", rule->GetLimitRule());
		mQuery.bind(":rulename", rule->GetRuleName());
		mQuery.bind(":interval", rule->GetInterval());
		mQuery.bind(":ruleid", ruleID);
		mQuery.exec();
		closeTable();
		return true;
	}

	int RuleService::getRuleCount() {
		const char* sql;
		sql = SQL_GetRuleCount;
		openTable();
		SQLite::Statement mQuery(db, sql);
		if (mQuery.executeStep())
		{
			closeTable();
			return mQuery.getColumn(0);
		}
		else
		{
			closeTable();
			return -1;
		}
	}

	bool RuleService::getAllRule(DB::TimeControllerRule*** rules,int& ruleCount) {
		const char* sql;
		sql = SQL_GetAllRule;
		openTable();
		SQLite::Statement mQuery(db, sql);
		ruleCount=db.execAndGet(SQL_GetRuleCount);
		int count=0;
		*rules = new DB::TimeControllerRule* [ruleCount];
		//vector<TimeControllerRule*> a=vector<TimeControllerRule*>();       
		while (mQuery.executeStep()&& count<ruleCount)
		{
			//DB::TimeControllerRule* r=new DB::TimeControllerRule; 

			(*rules)[count] = new DB::TimeControllerRule;
			(*rules)[count]->SetId(mQuery.getColumn(0));
			(*rules)[count]->SetRuleName(mQuery.getColumn(1));
			(*rules)[count]->SetProgramName(mQuery.getColumn(2));
			(*rules)[count]->SetProgramTitle(mQuery.getColumn(3));
			(*rules)[count]->SetProgramDirectory(mQuery.getColumn(4));
			(*rules)[count]->SetRunPath(mQuery.getColumn(5));
			(*rules)[count]->SetRunMode(mQuery.getColumn(6).getInt());
			//struct timeval t;
			//t.tv_sec = mQuery.getColumn(7).getInt64();
			//t.tv_usec = mQuery.getColumn(8).getInt64();
			(*rules)[count]->SetStartTime(mQuery.getColumn(7).getInt64());
			//t.tv_sec = mQuery.getColumn(9).getInt64();
			//t.tv_usec = mQuery.getColumn(10).getInt64();
			(*rules)[count]->SetEndTime(mQuery.getColumn(8).getInt64());
			(*rules)[count]->SetPerPeriodTime(mQuery.getColumn(9));
			(*rules)[count]->SetTimes(mQuery.getColumn(10));
			//t.tv_sec = mQuery.getColumn(13).getInt64();
			//t.tv_usec = mQuery.getColumn(14).getInt64();
			(*rules)[count]->SetTotalTime(mQuery.getColumn(11).getInt64());
			(*rules)[count]->SetLimitRule((DB::LimitRule)mQuery.getColumn(12).getInt());
			(*rules)[count]->SetInterval(mQuery.getColumn(13).getInt64());
			//rule=&r;
			count++;
		}
		closeTable();
		return (ruleCount>count);
		//rule = &a;
	}
}
