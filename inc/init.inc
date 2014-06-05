<?php
require_once 'inc/bo_user.class.inc';

if($_SERVER['HTTP_HOST'] == 'is-project.solution-set.net') {
	$db = mysql_connect(DB_HOST, DB_USER, DB_PASS) or die("Could not connect to mysql server.");
	mysql_select_db(DB_NAME, $db) or die('Could not select database.');
} elseif ($_SERVER['HTTP_HOST'] == '127.0.0.1') {
	$db = mysql_connect('localhost', 'root') or die("Could not connect to mysql server.");
	mysql_select_db('is-project', $db) or die('Could not select database.');
}

// TODO: Session & Login und so
global $current_user;
$current_user = new bo_user(0);


?>