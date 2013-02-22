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
generateAsserts(3);
echo implode("\n", $generated);

function getNumber()
{
	while(true) {
		$r = rand() % 1000;
		if(0 != $r) {
			return $r;
		}
	}
}

function generateAsserts($depth, $pre = '')
{
	global $operators, $generated;
	
	if('' == $pre) {
		$pre = getNumber();
	}
	
    foreach($operators as $sql => $c) {
		if($depth > 1) {
			generateAsserts($depth - 1, "$pre $c " . getNumber());
		}
		else {
    		$expr = "$pre $c " . getNumber();
			$generated[] = "    OperatorSuite_testOperator(db, \"$expr\", $expr);";
    	}
    }
}
