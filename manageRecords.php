<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageRecords.class.inc';
require_once 'inc/bo_collection.class.inc';
require_once 'inc/bo_record.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageCollections();

if(!isset($current_project) || $current_project->getProject() <= 0) $layout->_goto('manageProjects.php');

$layout->setActiveMenuTrail('project-records');


$collection = new bo_collection(2);

var_export($collection);

$data = array();
$data['rs'] = $current_project->getRecordStructure();
$data['records'] = array();
foreach ($collection->getRecords() as $record) {
	$record = new bo_record($current_project->getProject(), $record);

	$data['records'][] = $record->getRecord();
}

$layout->showManageRecordStructureForm( $data );

$layout->_print();
?>