<?php
require_once 'inc/bo_group.class.inc';

print 'hello world<hr>';

$variable = new bo_group(rand(0,2));

$metaData = $variable->getGroupMetaData();

print '<hr><pre>';
var_export($metaData); //var_export gibt alle Inhalte des Parameters aus
print '</pre><hr>';

print 'Gruppenname = '.$metaData['name'];

print '<hr>FUSSZEILE';

?>