<?php
if(!file_exists ('config.php')) {
	die('File "config.php" not found.');
}
require_once 'config.php';
require_once 'inc/bo_user.class.inc';

$db = mysql_connect(DB_HOST, DB_USER, DB_PASS) or die("Could not connect to mysql server.");
mysql_select_db(DB_NAME, $db) or die('Could not select database.');

// TODO: Session & Login und so
global $current_user;
$current_user = new bo_user(1);

?>