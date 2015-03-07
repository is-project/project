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
		case 'submitEditRecordStructureForm':
			submitEditRecordStructureForm($layout);
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


function submitEditRecordStructureForm($layout) {
	global $current_user;
	global $current_project;

	if(!isset($_POST['record-structure'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	$old_rs = $current_project->getRecordStructure();

	$data = json_decode( $_POST['record-structure'] );
	$new_rs = array();
	foreach ($data as $weight => $col) {
		if(is_null($col[0])) $col[0] = count($new_rs);
		$new_rs[ $col[0] ] = array(
				'col_name' => $col[0],
				'title' => $col[1],
				'type' => $col[2],
				'length' => $col[3],
				'decimal_places' => $col[4],
				'weight' => $weight,
			);
	}

	switch($current_project->setRecordStructure( $new_rs )) {
		case ERROR_PROJECT_EDIT_RECORD_STRUCTURE: 
			$layout->toast('##Error while editing Record Structure.##', 'error');
			$layout->addJS('settings', array('data2' => $new_rs));

			$data = array();
			foreach ($new_rs as $col) {
				$data[] = array(
						$col['col_name'],
						$col['title'],
						$col['type'],
						isset($col['length']) ? $col['length'] : NULL,
						isset($col['decimal_places']) ? $col['decimal_places'] : NULL,
					);
			}
			$layout->addJS('settings', array('data2' => $data));
			break;
		default:
			$project->saveProject();
			$layout->toast('##Record Structure was edited successfully.##');
	}

}

?>