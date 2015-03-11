<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageCollections.class.inc';
require_once 'inc/bo_collection.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageCollections();

if(!isset($current_project) || $current_project->getProject() <= 0) $layout->_goto('manageProjects.php');

$layout->setActiveMenuTrail('project-collections');

if(isset($_GET['action'])) {

	switch($_GET['action']) {
		case 'submitAddEditCollectionForm':
			submitAddEditCollectionForm($layout);
			break;
		case 'submitDeleteCollectionForm':
			submitDeleteCollectionForm($layout);
			break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}


$collections = $current_user->getListOfCollections( $current_project->getProject() );

$data = array();
foreach ($collections as $collection) {
	$collection = new bo_collection($collection);

	$data[] = $collection->getCollectionMetaData();
}

$layout->showManageRecordStructureForm( $data );

$layout->_print();

function submitAddEditCollectionForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['collection'], $_POST['name'], $_POST['description'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$errors = array();

	if($_POST['collection'] == '')
		$collection = new bo_collection(0, $current_project->getProject());
	else
		$collection = new bo_collection($_POST['collection']);

	switch ($collection->setName($_POST['name'])) {
		case ERROR_INVALID_COLLECTION_NAME:
			$errors[] = '##Name cannot be empty.##';
	}

	switch ($collection->setDescription($_POST['description'])) {
		
	}

	if(count($errors)) {
		$layout->addJS('settings', array('form-errors' => $errors));
		$layout->addJS('settings', array('form-status' => 'add'));
		$layout->addJS('settings', array('form-values' => array(
			'name' => $_POST['name'],
			'description' => $_POST['description'],
			'collection' => $_POST['collection']
			)));
	} else {
		$collection->saveCollection();
	}
}

function submitDeleteCollectionForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['collections'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$collections = json_decode($_POST['collections']);
	foreach ($collections as $collection) {
		$tmp = new bo_collection($collection);
		$tmp->deleteCollection();
	}
}

?>