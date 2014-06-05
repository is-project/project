<?php
require_once 'config.php';
require_once 'inc/init.inc';
require_once 'inc/bo_project.class.inc';

print 'hello world2<hr>';

$project = new bo_project(rand(0,4));

print '<hr><pre>';
var_export($project);
print '</pre><hr>';

print '<hr><pre>';
var_export($project->getRecordStructure());
print '</pre><hr>';

print '<hr>FUSSZEILE';

?>