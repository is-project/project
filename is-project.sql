-- phpMyAdmin SQL Dump
-- version 4.0.4
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Erstellungszeit: 18. Jun 2014 um 16:03
-- Server Version: 5.5.31-log
-- PHP-Version: 5.4.16

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Datenbank: `is-project`
--
CREATE DATABASE IF NOT EXISTS `is-project` DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci;
USE `is-project`;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `groups`
--

DROP TABLE IF EXISTS `groups`;
CREATE TABLE IF NOT EXISTS `groups` (
  `group` int(11) NOT NULL AUTO_INCREMENT,
  `project` int(11) NOT NULL,
  `name` varchar(45) NOT NULL,
  `system` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`group`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `groups`
--

INSERT INTO `groups` (`group`, `project`, `name`, `system`) VALUES
(1, 1, 'Project Leader', 1),
(2, 1, 'Guest', 2),
(3, 1, 'Guest (TU intranet)', 3),
(4, 2, 'Project Leader', 1),
(5, 2, 'Guest', 2),
(6, 2, 'Guest (TU intranet)', 3),
(7, 3, 'Project Leader', 1),
(8, 3, 'Guest', 2),
(9, 3, 'Guest (TU intranet)', 3),
(10, 4, 'Project Leader', 1),
(11, 4, 'Guest', 2),
(12, 4, 'Guest (TU intranet)', 3);

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `link_groups_permissions`
--

DROP TABLE IF EXISTS `link_groups_permissions`;
CREATE TABLE IF NOT EXISTS `link_groups_permissions` (
  `group` int(11) NOT NULL,
  `permission` int(11) NOT NULL,
  PRIMARY KEY (`group`,`permission`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `link_groups_permissions`
--

INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES
(1, 1),
(1, 2),
(1, 3),
(1, 4),
(4, 1),
(4, 2),
(4, 3),
(4, 4),
(7, 1),
(7, 2),
(7, 3),
(8, 1),
(10, 1),
(10, 2),
(10, 3);

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `link_users_groups`
--

DROP TABLE IF EXISTS `link_users_groups`;
CREATE TABLE IF NOT EXISTS `link_users_groups` (
  `user` int(11) NOT NULL,
  `group` int(11) NOT NULL,
  PRIMARY KEY (`user`,`group`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `link_users_groups`
--

INSERT INTO `link_users_groups` (`user`, `group`) VALUES
(1, 1),
(2, 4),
(3, 7),
(4, 10);

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `permissions`
--

DROP TABLE IF EXISTS `permissions`;
CREATE TABLE IF NOT EXISTS `permissions` (
  `permission` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `description` text NOT NULL,
  PRIMARY KEY (`permission`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `permissions`
--

INSERT INTO `permissions` (`permission`, `name`, `description`) VALUES
(1, 'view_project_metadata', 'View the projects meta data (name, description, parent project).'),
(2, 'edit_project_metadata', 'Edit the projects meta data (name, description, parent project).'),
(3, 'create_child_project', 'Create a new child/sub project'),
(4, 'delete_child_project', 'Delete a child/sub project');

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `projects`
--

DROP TABLE IF EXISTS `projects`;
CREATE TABLE IF NOT EXISTS `projects` (
  `project` int(11) NOT NULL AUTO_INCREMENT,
  `parent_project` int(11) NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `created_by` int(11) NOT NULL,
  `name` varchar(45) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `description` text CHARACTER SET utf8 COLLATE utf8_bin,
  `record_structure` text CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`project`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `projects`
--

INSERT INTO `projects` (`project`, `parent_project`, `created`, `created_by`, `name`, `description`, `record_structure`) VALUES
(1, 0, '0000-00-00 00:00:00', 0, 'TUBAF', 'Root Project', '[{"col_name":"record","title":"Record","type":"int","weight":0},{"col_name":"deleted","title":"Deleted","type":"timestamp","weight":1},{"col_name":"deleted_by","title":"Deleted By","type":"int","weight":2},{"col_name":"created","title":"Created","type":"timestamp","weight":3},{"col_name":"created_by","title":"Created By","type":"int","weight":4}]'),
(2, 1, '0000-00-00 00:00:00', 0, 'Physik', 'Projekt fÃ¼r den Fachbereich Physik', '[{"col_name":"record","title":"Record","type":"int","weight":0},{"col_name":"deleted","title":"Deleted","type":"timestamp","weight":1},{"col_name":"deleted_by","title":"Deleted By","type":"int","weight":2},{"col_name":"created","title":"Created","type":"timestamp","weight":3},{"col_name":"created_by","title":"Created By","type":"int","weight":4}]'),
(3, 1, '0000-00-00 00:00:00', 0, 'Chemie', 'Projekt fÃ¼r den Fachbereich Chemie', '[{"col_name":"record","title":"Record","type":"int","weight":0},{"col_name":"deleted","title":"Deleted","type":"timestamp","weight":1},{"col_name":"deleted_by","title":"Deleted By","type":"int","weight":2},{"col_name":"created","title":"Created","type":"timestamp","weight":3},{"col_name":"created_by","title":"Created By","type":"int","weight":4}]'),
(4, 2, '0000-00-00 00:00:00', 0, 'Pyromaterialien', 'Untersuchngen zu Pyromaterialien im Fachbereich Physik', '[{"col_name":"record","title":"Record","type":"int","weight":0},{"col_name":"deleted","title":"Deleted","type":"timestamp","weight":1},{"col_name":"deleted_by","title":"Deleted By","type":"int","weight":2},{"col_name":"created","title":"Created","type":"timestamp","weight":3},{"col_name":"created_by","title":"Created By","type":"int","weight":4}]');

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `users`
--

DROP TABLE IF EXISTS `users`;
CREATE TABLE IF NOT EXISTS `users` (
  `user` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `email` varchar(45) NOT NULL,
  `password` varchar(128) NOT NULL,
  `description` text,
  `valid_until` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`user`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `users`
--

INSERT INTO `users` (`user`, `name`, `email`, `password`, `description`, `valid_until`) VALUES
(1, 'Max MÃ¼ller', 'max.mueller@solution-set.net', '1', NULL, NULL),
(2, 'Sabine ZÃ¶llner', 'sabine.zoellner@solution-set.net', '1', NULL, NULL),
(3, 'Frank Fass', 'frank.fass@solution-set.net', '1', NULL, NULL),
(4, 'Sarah Hunsk', 'sarah.hunsk@solution-set.net', '1', NULL, NULL),
(5, 'Martin BÃ¤r', 'martin.baer@solution-set.net', '1', NULL, NULL);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
