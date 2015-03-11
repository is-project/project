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

if(isset($_GET['collection'])) {
	$collection = new bo_collection($_GET['collection']);
	$layout->setAreaContent('collection', 'Collection: '.$collection->getName());
} else {

	if(isset($_GET['action'])) {

		switch($_GET['action']) {
			case 'submitAddEditRecordForm':
				submitAddEditRecordForm($layout);
				break;
			case 'submitDeleteRecordForm':
				submitDeleteRecordForm($layout);
				break;
			case 'submitAddRecordToCollectionForm':
				submitAddRecordToCollectionForm($layout);
				break;
			
			default:
				$layout->toast('Wrong Action', 'error');
				break;
		}

	}

	$collection = new bo_collection(0, $current_project->getproject(), true);
	$layout->setAreaContent('collection', 'All records');
	
}


if($collection->getProject() == $current_project->getProject()) {
	$data = array();
	$data['rs'] = $current_project->getRecordStructure();
	$data['collection'] = $collection->getCollection();
	$data['records'] = array();
	foreach ($collection->getRecords() as $r) {
		$record = new bo_record($current_project->getProject(), $r);
		$tmp = $record->getRecord();
		if(isset($tmp['record']))
			$data['records'][] = $record->getRecord();
		else {
			$collection->unlinkRecord($r);
			$collection->saveCollection();
		}
	}

	if($collection->getCollection() == -1) {
		$data['collections'] = array();
		foreach ($current_user->getListOfCollections( $current_project->getProject() ) as $collection) {
			$tmp = new bo_collection($collection);
			// var_export($tmp);
			$data['collections'][$tmp->getCollection()] = $tmp->getName();
		}
	}

	$layout->showManageRecordStructureForm( $data );
} else {
	$layout->toast('##Access Denied##', 'error');
}



$layout->_print();

function submitAddEditRecordForm($layout) {
	global $current_user;
	global $current_project;


	if(!isset($_POST['record'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$errors = array();

	if($_POST['record'] == '')
		$record = new bo_record($current_project->getProject(), 0, $_POST);
	else {
		$record = new bo_record($current_project->getProject(), $_POST['record']);		
		$record->setParams($_POST);
	}

	$record->saveRecord();

}

function submitDeleteRecordForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['records'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$records = json_decode($_POST['records']);
	foreach ($records as $record) {
		$tmp = new bo_record($current_project->getProject(), $record);
		$tmp->deleteRecord();
	}
}

function submitAddRecordToCollectionForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['records'],$_POST['collection'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$collection = new bo_collection($_POST['collection']);
	// var_export($collection);

	$records = json_decode($_POST['records']);
	foreach ($records as $record) {
		$collection->linkRecord($record);
	}
	$collection->saveCollection();
}
?>
