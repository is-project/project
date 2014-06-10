<?php
ini_set ('display_errors', '1');
error_reporting(E_ALL);
require_once 'inc/bo_group.class.inc';

print 'hello world<hr>';

$variable = new bo_group(rand(0,2));

$metaData = $variable->getGroupMetaData();
$permission = $variable->getPermissions();
$member = $variable->getMembers();


print '<hr><pre>';
var_export($metaData); //var_export gibt alle Inhalte des Parameters aus
print '</pre><hr>';

print 'Gruppenname = '.$metaData['name'];
print '<hr><pre>';
print 'member:';
var_export($member); //var_export gibt alle Inhalte des Parameters aus
print '</pre><hr>'; 
print '<hr><pre>';
print 'permission:';
var_export($permission); //var_export gibt alle Inhalte des Parameters aus
print '</pre><hr>';
print $variable->setName('horst');
print $variable->setDescription('ulme');
print '<hr>FUSSZEILE';

?>