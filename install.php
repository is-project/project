<?php

if(!file_exists('config.php')) {
	die('File "config.php" not found.');
}

require_once 'config.php';

$db = mysql_connect(DB_HOST, DB_USER, DB_PASS) or die("Could not connect to mysql server.");
mysql_select_db(DB_NAME, $db) or die('Could not select database.');

if(isset($_GET['action']) && ($_GET['action'] == 'install' || $_GET['action'] == 'install_all') ) {

	$test_data = false;
	if($_GET['action'] == 'install_all') $test_data = true;

	#######################################

	##
	## Table structure for table `groups`
	##

	mysql_query("DROP TABLE IF EXISTS `groups`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `groups` (
	  `group` int(11) NOT NULL AUTO_INCREMENT,
	  `project` int(11) NOT NULL,
	  `name` varchar(45) NOT NULL,
	  `system` int(11) NOT NULL DEFAULT '0',
	  PRIMARY KEY (`group`)
	) ENGINE=InnoDB  DEFAULT CHARSET=utf8;");

	##
	## Dumping data for table `groups`
	##

	mysql_query("INSERT INTO `groups` (`group`, `project`, `name`, `system`) VALUES
	(1, 1, 'Project Leader', 1),
	(2, 1, 'Guest', 2),
	(3, 1, 'Guest (TU intranet)', 3);");

	if($test_data)
		mysql_query("INSERT INTO `groups` (`group`, `project`, `name`, `system`) VALUES
		(4, 2, 'Project Leader', 1),
		(5, 2, 'Guest', 2),
		(6, 2, 'Guest (TU intranet)', 3),
		(7, 3, 'Project Leader', 1),
		(8, 3, 'Guest', 2),
		(9, 3, 'Guest (TU intranet)', 3),
		(10, 4, 'Project Leader', 1),
		(11, 4, 'Guest', 2),
		(12, 4, 'Guest (TU intranet)', 3);");

	#######################################

	##
	## Table structure for table `link_groups_permissions`
	##

	mysql_query("DROP TABLE IF EXISTS `link_groups_permissions`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `link_groups_permissions` (
	  `group` int(11) NOT NULL,
	  `permission` int(11) NOT NULL,
	  PRIMARY KEY (`group`,`permission`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;");

	##
	## Dumping data for table `link_groups_permissions`
	##

	mysql_query("INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES
	(1, 1),
	(1, 2),
	(1, 3),
	(1, 4),
	(1, 5),
	(1, 6),
	(1, 7),
	(1, 8);");

	if($test_data)
		mysql_query("INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES
		(4, 1),
		(4, 2),
		(4, 3),
		(4, 4),
		(4, 5),
		(4, 6),
		(4, 7),
		(4, 8),
		(7, 1),
		(7, 2),
		(7, 3),
		(7, 4),
		(7, 5),
		(7, 6),
		(7, 7),
		(7, 8),
		(8, 1),
		(10, 1),
		(10, 2),
		(10, 3),
		(10, 4),
		(10, 5),
		(10, 6),
		(10, 7),
		(10, 8);");

	#######################################

	##
	## Table structure for table `link_users_groups`
	##

	mysql_query("DROP TABLE IF EXISTS `link_users_groups`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `link_users_groups` (
	  `user` int(11) NOT NULL,
	  `group` int(11) NOT NULL,
	  PRIMARY KEY (`user`,`group`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;");

	##
	## Dumping data for table `link_users_groups`
	##

	mysql_query("INSERT INTO `link_users_groups` (`user`, `group`) VALUES
	(1, 1);");

	if($test_data)
		mysql_query("INSERT INTO `link_users_groups` (`user`, `group`) VALUES
		(2, 4),
		(3, 7),
		(4, 10);");

	#######################################

	##
	## Table structure for table `permissions`
	##

	mysql_query("DROP TABLE IF EXISTS `permissions`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `permissions` (
	  `permission` int(11) NOT NULL AUTO_INCREMENT,
	  `name` varchar(45) NOT NULL,
	  `description` text NOT NULL,
	  PRIMARY KEY (`permission`)
	) ENGINE=InnoDB  DEFAULT CHARSET=utf8;");

	##
	## Dumping data for table `permissions`
	##

	mysql_query("INSERT INTO `permissions` (`permission`, `name`, `description`) VALUES
	(1, 'view_project_metadata', 'View the projects meta data (name, description, parent project).'),
	(2, 'edit_project_metadata', 'Edit the projects meta data (name, description, parent project).'),
	(3, 'create_child_project', 'Create a new child/sub project'),
	(4, 'delete_child_project', 'Delete a child/sub project'),
	(5, 'edit_record_structure', 'Edit the record structure for all records containing to this project.'),
	(6, 'manage_groups', 'Permission to view / edit / create / delete groups in a project. This includes also the user management for all groups in the given project.'),
	(7, 'view_records', 'View records in a project and manage them in collections'),
	(8, 'edit_records', 'Add / Edit / Delete records in a project');");

	#######################################

	##
	## Table structure for table `projects`
	##

	mysql_query("DROP TABLE IF EXISTS `projects`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `projects` (
	  `project` int(11) NOT NULL AUTO_INCREMENT,
	  `parent_project` int(11) NOT NULL,
	  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
	  `created_by` int(11) NOT NULL,
	  `name` varchar(45) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
	  `description` text CHARACTER SET utf8 COLLATE utf8_bin,
	  `record_structure` text CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
	  PRIMARY KEY (`project`)
	) ENGINE=InnoDB  DEFAULT CHARSET=utf8;");

	##
	## Dumping data for table `projects`
	##

	mysql_query("INSERT INTO `projects` (`project`, `parent_project`, `created_by`, `name`, `description`, `record_structure`) VALUES
	(1, 0, 0, 'TUBAF', 'Root Project', '[]');");
	
	if($test_data)
		mysql_query('INSERT INTO `projects` (`project`, `parent_project`, `created_by`, `name`, `description`, `record_structure`) VALUES
		(2, 1, 1, \'Physik\', \'Projekt für den Fachbereich Physik\', \'[]\'),
		(3, 1, 1, \'Chemie\', \'Projekt für den Fachbereich Chemie\', \'[]\'),
		(4, 2, 2, \'Pyromaterialien\', \'Untersuchngen zu Pyromaterialien im Fachbereich Physik\', \'[{\"title\":\"Name\",\"type\":\"text\",\"length\":\"128\",\"decimal_places\":\"\",\"weight\":0,\"col_name\":\"param2\"},{\"title\":\"Ordnungszahl\",\"type\":\"int\",\"length\":\"\",\"decimal_places\":\"\",\"weight\":1,\"col_name\":\"param3\"},{\"title\":\"Hinzugef\\\\u00fcgt am\",\"type\":\"timestamp\",\"length\":\"\",\"decimal_places\":\"\",\"weight\":2,\"col_name\":\"param4\"},{\"title\":\"Dichte $\\\\\\\\rho~~in~~\\\\\\\\frac{g}{cm^3}$\",\"type\":\"double\",\"length\":\"\",\"decimal_places\":\"3\",\"col_name\":\"param1\",\"weight\":3},{\"title\":\"Explosiv\",\"type\":\"boolean\",\"length\":\"\",\"decimal_places\":\"\",\"weight\":4,\"col_name\":\"param5\"}]\');');

	#######################################

	##
	## Table structure for table `data_project_n`
	##
	mysql_query("CREATE TABLE IF NOT EXISTS `data_project_1` (
		`entry` int(11) NOT NULL AUTO_INCREMENT,
		  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
		  `created_by` int(11) NOT NULL,
		  `deleted` timestamp NULL DEFAULT NULL,
		  `deleted_by` int(11) DEFAULT NULL,
		  PRIMARY KEY (`entry`)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;");

	if($test_data) {
		mysql_query("CREATE TABLE IF NOT EXISTS `data_project_2` (
			`entry` int(11) NOT NULL AUTO_INCREMENT,
			  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
			  `created_by` int(11) NOT NULL,
			  `deleted` timestamp NULL DEFAULT NULL,
			  `deleted_by` int(11) DEFAULT NULL,
			  PRIMARY KEY (`entry`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;");

		mysql_query("CREATE TABLE IF NOT EXISTS `data_project_3` (
			`entry` int(11) NOT NULL AUTO_INCREMENT,
			  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
			  `created_by` int(11) NOT NULL,
			  `deleted` timestamp NULL DEFAULT NULL,
			  `deleted_by` int(11) DEFAULT NULL,
			  PRIMARY KEY (`entry`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;");

		mysql_query("CREATE TABLE IF NOT EXISTS `data_project_4` (
			`entry` int(11) NOT NULL AUTO_INCREMENT,
			  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
			  `created_by` int(11) NOT NULL,
			  `deleted` timestamp NULL DEFAULT NULL,
			  `deleted_by` int(11) DEFAULT NULL,
			  `param1` double NOT NULL,
			  `param2` varchar(128) COLLATE utf8_unicode_ci NOT NULL,
			  `param3` int(11) NOT NULL,
			  `param4` timestamp NULL DEFAULT NULL,
			  `param5` tinyint(1) NOT NULL,
			  PRIMARY KEY (`entry`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;");

		##
		## Daten für Tabelle `data_project_4`
		##

		mysql_query("INSERT INTO `data_project_4` (`entry`, `created`, `created_by`, `deleted`, `deleted_by`, `param1`, `param2`, `param3`, `param4`, `param5`) VALUES
		(1, '2015-03-09 21:19:16', 1, NULL, NULL, 7.87, 'Eisen', 26, '2015-03-07 23:00:00', 0),
		(2, '2015-03-09 21:20:21', 1, NULL, NULL, 0.971, 'Natrium', 11, '2015-03-01 23:00:00', 1),
		(3, '2015-03-09 21:22:21', 1, NULL, NULL, 1.738, 'Magnesium', 12, '2015-03-03 23:00:00', 0);");
	}

	##
	## Table structure for table `users`
	##

	mysql_query("DROP TABLE IF EXISTS `users`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `users` (
	  `user` int(11) NOT NULL AUTO_INCREMENT,
	  `name` varchar(45) NOT NULL,
	  `email` varchar(45) NOT NULL UNIQUE,
	  `password` varchar(128) NOT NULL,
	  `description` text,
	  `valid_until` timestamp NULL DEFAULT NULL,
	  PRIMARY KEY (`user`),
	  UNIQUE KEY `name` (`name`)
	) ENGINE=InnoDB  DEFAULT CHARSET=utf8;");

	##
	## Tabellenstruktur für Tabelle `collections`
	##

	mysql_query("DROP TABLE IF EXISTS `collections`;");
	mysql_query("CREATE TABLE IF NOT EXISTS `collections` (
	`collection` int(11) NOT NULL AUTO_INCREMENT,
	  `name` varchar(45) NOT NULL,
	  `description` text NULL DEFAULT NULL,
	  `user` int(11) NOT NULL,
	  `project` int(11) NOT NULL,
	  `records` text NOT NULL,
	  PRIMARY KEY (`collection`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;");

	##
	## Daten für Tabelle `collections`
	##
	if($test_data)
		mysql_query("INSERT INTO `collections` (`collection`, `name`, `description`, `user`, `project`, `records`) VALUES
		(1, 'hi', 'hoy', 1, 4, '[]'),
		(2, 'Public', 'Öffentlich Einsichtige Datensätze', 1, 4, '[2,3]');");

	##
	## Dumping data for table `users`
	##

	mysql_query("INSERT INTO `users` (`user`, `name`, `email`, `password`, `description`, `valid_until`) VALUES
	(1, 'admin', 'admin@solution-set.net', '3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2', NULL, NULL);");

	if($test_data)
		mysql_query("INSERT INTO `users` (`user`, `name`, `email`, `password`, `description`, `valid_until`) VALUES
		(2, 'Max Müller', 'max.mueller@solution-set.net', '3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2', NULL, NULL),
		(3, 'Sabine Zöllner', 'sabine.zoellner@solution-set.net', '3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2', NULL, NULL),
		(4, 'Frank Fass', 'frank.fass@solution-set.net', '3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2', NULL, NULL),
		(5, 'Sarah Hunsk', 'sarah.hunsk@solution-set.net', '3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2', NULL, NULL),
		(6, 'Martin Bär', 'martin.baer@solution-set.net', '3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2', NULL, NULL);");


	#######################################

	print 'Done';
} else {
	print '<a href="?action=install">Install without test data</a> | <a href="?action=install_all">Install with test data</a>';
}

?>