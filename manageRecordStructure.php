<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageRecordStructure.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageRecordStructure();

if(!isset($current_project) || $current_project->getProject() <= 0) {
	$layout->_goto('manageProjects.php');
}

$layout->setActiveMenuTrail('project-record-structure');

$layout->showManageRecordStructureForm();

$layout->_print();

?>