#!/usr/bin/expect

set timeout 2
set sql_i 1

set sql(1)   "create table mytable ( col1 int )"
set sql_r(1) "Table \"default.mytable\" created."
set sql(2)   "select col1 from mytable"
set sql_r(2) {.*0 records.*}
set sql(3)   "insert into mytable (col1) values (123)"
set sql_r(3) {.*}
set sql(4)   "select col1 from mytable"
set sql_r(4) {.*1 record.*}
set sql(5)   "select table_schema, table_name from information_schema_tables"
set sql_r(5) {.* records.*}

spawn "/Users/elliot/Library/Developer/Xcode/DerivedData/eagle-bvzxfkxjojdzztarraazawlgiejc/Build/Products/Debug/eagle"

proc run_sql {} {
	global sql_i sql sql_r
	
	send "$sql($sql_i);\r"
	expect {
		-re $sql_r($sql_i) {
			# next command
			incr sql_i
			if {[info exists sql($sql_i)]} {
				run_sql
			}
		}
		timeout {
			exit 1
		}
	}
}

run_sql
puts "\n"
exit
