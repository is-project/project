<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageRecordStructure.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageRecordStructure();

if(!isset($current_project) || $current_project->getProject() <= 0) $layout->_goto('manageProjects.php');
if( !$current_user->access('edit_record_structure', $current_project->getProject()) ) die("Permission Denied");

$layout->setActiveMenuTrail('project-record-structure');

if(isset($_GET['action'])) {

	switch($_GET['action']) {
		case 'submitAddEditRecordStructureForm':
			submitAddEditRecordStructureForm($layout);
			break;
		case 'submitDeleteRecordStructureForm':
			submitDeleteRecordStructureForm($layout);
			break;
		case 'submitOrderForm':
			submitOrderForm($layout);
			break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

$rs = $current_project->getRecordStructure();
$data = array();
foreach ($rs as $col) {
	$data[] = array(
			$col->col_name,
			$col->title,
			$col->type,
			isset($col->length) ? $col->length : NULL,
			isset($col->decimal_places) ? $col->decimal_places : NULL,
		);
}

$layout->showManageRecordStructureForm($data);
$layout->_print();


function submitAddEditRecordStructureForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['title'], $_POST['type'], $_POST['length'], $_POST['dec_places'], $_POST['col'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$errors = $current_project->addEditRecordStructureColoumn($_POST);

	if(count($errors)) {
		$err_str = array();
		if(in_array(ERROR_RECORD_STRUCTURE_TITLE_NO_STRING, $errors))
			$err_str[] = '##Title must be a string.##';
		if(in_array(ERROR_RECORD_STRUCTURE_TITLE_EMPTY, $errors))
			$err_str[] = '##Title cannot be empty.##';
		if(in_array(ERROR_RECORD_STRUCTURE_TYPE_INVALID, $errors))
			$err_str[] = '##Invalid type selection.##';
		if(in_array(ERROR_RECORD_STRUCTURE_LENGTH_INVALID, $errors))
			$err_str[] = '##Length must be an integer.##';
		if(in_array(ERROR_RECORD_STRUCTURE_DECPLACES_INVALID, $errors))
			$err_str[] = '##Decimal Places must be an integer.##';
		if(in_array(ERROR_RECORD_STRUCTURE_COL_INVALID, $errors))
			$err_str[] = '##Access Denied.##';

		$layout->addJS('settings', array('form-errors' => $err_str));
		$layout->addJS('settings', array('form-status' => 'add'));
		$layout->addJS('settings', array('form-values' => array(
			'title' => $_POST['title'],
			'type' => $_POST['type'],
			'length' => $_POST['length'],
			'dec_places' => $_POST['dec_places'],
			'col' => $_POST['col']
			)));

	} else {
		$current_project->saveProject();
		if($_POST['col'] == '')
			$layout->toast('##Record structure coloumn was added successfully.##');
		else
			$layout->toast('##Record structure coloumn was edited successfully.##');
	}
}

function submitDeleteRecordStructureForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['cols'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$errors = $current_project->deleteRecordStructureColoumn($_POST);

	if(count($errors)) {
		$err_str = array();
		if(in_array(ERROR_PROJECT_ACCESS_DENIED, $errors))
			$err_str[] = '##Access Denied.##';
	} else {
		$current_project->saveProject();
		$layout->toast('##Record structure coloumn(s) was/were deleted successfully.##');
	}

}

function submitOrderForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['order'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$errors = $current_project->orderRecordStructureColoumn($_POST);

	if(count($errors)) {
		$layout->toast('##Error##');
	} else {
		$current_project->saveProject();
		$layout->toast('##Record structure order was edited successfully.##');
	}

}

?>