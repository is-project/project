<?php
ini_set ('display_errors', '1');
error_reporting(E_ALL);
require_once 'inc/bo_register.class.inc';

print 'hello world<hr>';

$register = new bo_register(rand(0,2));

//for Testing:
$metaData = $register->getRegisterMetaData();
//$permission = $register->getPermissions();
//$struktur = $register->getMembers();


print '<hr><pre>';
var_export($metaData); //var_export gibt alle Inhalte des Parameters aus
print '</pre><hr>';

print 'Registername = '.$metaData['name'];
print '<hr><pre>';

// print 'member:';
// var_export($member); //var_export gibt alle Inhalte des Parameters aus
// print '</pre><hr>'; 
// print '<hr><pre>';
// print 'permission:';
// var_export($permission); //var_export gibt alle Inhalte des Parameters aus
// print '</pre><hr>';
print $register->setName('horst');
print '<hr><pre>';
print $register->setDescription('ulme');
print '<hr>FUSSZEILE';

?>