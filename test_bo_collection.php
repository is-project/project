<?php
require_once 'inc/bo_collection.class.inc';

print 'hello world<hr>';

$collection = new bo_collection(rand(0,4));

print '<hr><pre>';
var_export($collection);
print '</pre><hr>';

print '<hr><pre>';
var_export($collection->getRecords());
print '</pre><hr>';

$collection->linkRecord(665);
$collection->linkRecord(666);
$collection->linkRecord(667);
$collection->linkRecord(667);
$collection->linkRecord(667);
$collection->linkRecord(667);

print '<hr><pre>';
var_export($collection->getRecords());
print '</pre><hr>';

$collection->unlinkRecord(666);

print '<hr><pre>';
var_export($collection->getRecords());
print '</pre><hr>';

print '<hr>FUSSZEILE';

?>