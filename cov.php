<?php

$coverage = explode("\n", file_get_contents('coverage.info'));
$records = array();
for($i = 0; $i < count($coverage);) {
    $record = array();
    for(; $i < count($coverage); ++$i) {
        $record[] = $coverage[$i];
        if($coverage[$i] == 'end_of_record') {
            ++$i;
            break;
        }
    }
    $record = implode("\n", $record);
    if(strpos($record, '/eagle/test') === false) {
        $records[] = $record;
    }
}

file_put_contents('coverage.info', implode("\n", $records));
