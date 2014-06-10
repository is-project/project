<?php
require_once 'inc/bo_record.class.inc';
require_once 'config.php';

print 'hello world, it is '.time().'<br>';
$usingRecord = rand(0,3);


$BORecord = new bo_record($usingRecord);

$record = $BORecord->getRecord();

print '<hr><pre>';
var_export($record); 
print '</pre><hr>';

$testdata = array('Orange', '33', '5.1392');
print '<hr>Inserting testdata<pre>';
var_export($testdata);
$valid = $BORecord->setParams($testdata);
print '</pre>';
print('Inserting data successfull: '.$valid);
print '<hr>';

print '<hr>Trying to delete the record<pre>';
$valid = $BORecord->deleteRecord();
print '</pre>';
print('Deleting data successfull: '.$valid);
print '<hr>';

$BORecord2 = new bo_record($usingRecord);
$record = $BORecord2->getRecord();

print '<hr>The record loaded again:<pre>';
var_export($record); 
print '</pre><hr>';
?>