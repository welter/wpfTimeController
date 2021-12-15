#include "DBRuleService.h"
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

	SQLite::Database DBRuleService::openTable() {

		SQLite::Database db("");
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
				return NULL;
			}
		}

		if (!db.tableExists("RULE")) {
			const char* sql;
			s = SQL_CreateRuleTable;
			sql = s.c_str();
			db.exec(sql);
		}
		return(db);
	}


	int DBRuleService::addRule(TimeControllerRule* rule) {
		const char* sql;
		string r;
		SQLite::Database db = openTable();
		r = SQL_AddRule((*rule));
		sql = r.c_str();
		rc = db.exec(sql);
		return rc;
	}
	bool DBRuleService::deleteRule(int ruleID) {
		const char* sql;
		string r;
		SQLite::Database db = openTable();
		r = SQL_DeleteRule(to_string(ruleID));
		sql = r.c_str();
		rc = db.exec(sql);
		return rc;
	}
	bool DBRuleService::deleteRule(const char* ruleName) {
		const char* sql;
		string r;
		SQLite::Database db = openTable();
		r = SQL_DeleteRuleByName((string) ruleName);
		sql = r.c_str();
		rc = db.exec(sql);
		return rc;
	}
	bool DBRuleService::clear() {
		const char* sql;
		string r;
		SQLite::Database db = openTable();
		r = SQL_ClearRule;
		sql = r.c_str();
		rc = db.exec(sql);
		return rc;
	}
	bool DBRuleService::getRule(DB::TimeControllerRule* rule, const char* ruleName) {
		const char* sql;
		sql = SQL_GetRuleByName;
		SQLite::Database db = openTable();
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
		}
		return true;
		//rule = &a;
	}
	bool DBRuleService::getRule(DB::TimeControllerRule* rule, int ruleID) {
		const char* sql;
		sql = SQL_GetRuleByID;
		SQLite::Database db = openTable();
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
		}
		return true;
		//rule = &a;
	}

	bool DBRuleService::setRule(const char* ruleName, DB::TimeControllerRule* rule) {
		const char* sql;
		sql = SQL_SetRuleByName;
		SQLite::Database db = openTable();
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

		mQuery.bind(":rulename", ruleName);
		mQuery.exec();
		return true;
	}
	bool DBRuleService::setRule(int ruleID, DB::TimeControllerRule* rule) {
		const char* sql;
		sql = SQL_SetRuleByID;
		SQLite::Database db = openTable();
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

		mQuery.bind(":ruleid", ruleID);
		mQuery.exec();
		return true;
	}

}
