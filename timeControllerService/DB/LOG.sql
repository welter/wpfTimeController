/*
 Navicat SQLite Data Transfer

 Source Server         : log
 Source Server Type    : SQLite
 Source Server Version : 2008017 (2.8.17)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 2008017 (2.8.17)
 File Encoding         : 65001

 Date: 12/11/2023 11:42:25
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for LOG
-- ----------------------------
DROP TABLE "LOG";
CREATE TABLE "LOG" (
  "logTime" integer,
  "cntThreads" integer,
  "cntUsage" integer,
  "dwFlags" integer,
  "dwSize" integer,
  "pcPriClassBase" integer,
  "szExeFile" TEXT,
  "th32DefaultHeapID" INTEGER,
  "th32ModuleID" INTEGER,
  "th32ParentProcessID" integer,
  "th32ProcessID" INTEGER
);

PRAGMA foreign_keys = true;
