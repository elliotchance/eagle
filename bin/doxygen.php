<?php

function addError($desc)
{
	echo ">>> $desc\n";
}

function getReturnDesc($node)
{
	foreach($node->para as $n) {
		if($n->simplesect && $n->simplesect['kind'] == 'return') {
			return $n->simplesect->para;
		}
	}
}

function getParamList($node)
{
	foreach($node->para as $n) {
		if($n->parameterlist) {
			return $n->parameterlist->parameteritem;
		}
	}
}

function validateFile($file)
{
	$xml = simplexml_load_file($file);
	
	foreach($xml->compounddef->sectiondef->memberdef as $member) {
		// check for description
		$desc = trim((string) $member->briefdescription->para);
		if(!$desc) {
			addError("[{$member->location['file']}:{$member->location['line']}]: No description for {$member->name}{$member->argsstring}");
		}
		
		// check return
		if((string) $member->type && (string) $member->type != 'void') {
			$return = getReturnDesc($member->detaileddescription);
			if(!$return) {
				addError("[{$member->location['file']}:{$member->location['line']} {$member->name}{$member->argsstring}]: Missing @return description.");
			}
		}
		
		// check parameters
		$realparams = array();
		foreach($member->param as $param) {
			$realparams[] = (string) $param->declname;
		}
		
		$docparams = array();
		$paramlist = getParamList($member->detaileddescription);
		if($paramlist) {
			foreach($paramlist as $param) {
				if(!$param->parameternamelist->parametername['direction']) {
					addError("[{$member->location['file']}:{$member->location['line']} {$member->name}{$member->argsstring}]: Parameter '{$param->parameternamelist->parametername}' does not have a direction.");
				}
				$docparams[] = (string) $param->parameternamelist->parametername;
			}
		}
		
		foreach($realparams as $param) {
			if(!in_array($param, $docparams)) {
				addError("[{$member->location['file']}:{$member->location['line']} {$member->name}{$member->argsstring}]: Parameter '$param' is not documented.");
			}
		}
	}
}

function validateDirectory($dirname)
{
	foreach (scandir($dirname) as $file) {
		if(substr($file, 0, 1) == "." || substr($file, strlen($file) - 4, 4) != ".xml") {
			continue;
		}
		validateFile("$dirname/$file");
	}
}

validateDirectory("doc/xml");
