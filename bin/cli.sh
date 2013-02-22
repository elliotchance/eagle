#!/usr/bin/expect

set timeout 2

spawn "/Users/elliot/Library/Developer/Xcode/DerivedData/eagle-bvzxfkxjojdzztarraazawlgiejc/Build/Products/Debug/eagle"

proc run_sql {sql expected} {
	puts $sql
	expect "eagle> " {
		send "$sql;\r"
		expect {
			-re $expected {
				# do nothing
			}
			timeout {
				exit 1
			}
		}
	}
}

run_sql "create table mytable ( col1 int )" "Table 'mytable' created."
run_sql "select * from mytable" {.*: .USERERROR. mytable}

interact
