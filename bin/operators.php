<?php

/**
 This file generates the CUNIT tests for operator precendence.
 */

$operators = array(
    /* sql */       /* c */
    ">="       =>   ">=",
    "<="       =>   "<=",
    "!="       =>   "!=",
    ">"        =>   ">",
    "<"        =>   "<",
    "*"        =>   "*",
    "+"        =>   "+",
    "-"        =>   "-",
    "/"        =>   "/",
    "%"        =>   "%",
    "="        =>   "==",
);
$generated = array();

srand(0);
generateAsserts(2);

// generate new file
$file = 'eagle_test/suites/OperatorSuite.c';
$contents = file_get_contents($file);
$contents = preg_replace(',/\*~ operators ~\*/.*/\*~ /operators ~\*/,ms',
	"/*~ operators ~*/\n" . implode("\n", $generated) . "\n    /*~ /operators ~*/", $contents);
file_put_contents($file, $contents);

function getNumber()
{
	while(true) {
		$r = rand() % 1000;
		if(0 != $r) {
			return $r;
		}
	}
}

function generateAsserts($depth, $pre_sql = '', $pre_c = '')
{
	global $operators, $generated;
	
	if('' == $pre_sql) {
		$pre_sql = $pre_c = getNumber();
	}
	
    foreach($operators as $sql => $c) {
		if($depth > 1) {
			$n = getNumber();
			generateAsserts($depth - 1, "$pre_sql $sql $n", "$pre_c $c $n");
		}
		else {
			$n = getNumber();
			$generated[] = "    OperatorSuite_testOperator(db, \"$pre_sql $sql $n\", $pre_c $c $n);";
    	}
    }
}
