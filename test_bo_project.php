<?php

require_once 'inc/init.inc';
require_once 'inc/bo_project.class.inc';

print 'hello world2<hr>';

global $current_user;
$x = rand(0,4);
print "<h1>$x (user: {$current_user->getUser()})</h1>";
$project = new bo_project($x);

print '<hr><pre>';
var_export($project);
print '</pre><hr>';

print '<hr><pre>';
var_export($project->getRecordStructure());
print '</pre><hr>';

print '<hr><pre>';
var_export(json_encode( $project->getRecordStructure() ));
print '</pre><hr>';

print '<hr>FUSSZEILE';

?>