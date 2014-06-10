<?php

require_once 'inc/init.inc';
require_once 'inc/bo_user.class.inc';

print 'hello world2<hr>';

$user = new bo_user(rand(0,4));

print '<hr><pre>';
var_export($user);
print '</pre><hr>';

print '<hr>FUSSZEILE';

?>