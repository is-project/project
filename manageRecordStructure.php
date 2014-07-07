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
		$new_rs[ $col[0] ] = array(
				'col_name' => $col[0],
				'title' => $col[1],
				'type' => $col[2],
				'length' => $col[3],
				'decimal_places' => $col[4],
				'weight' => $weight,
			);
	}

	print '<hr><pre>';
	var_export( $new_rs );
	print '</pre><hr>';

	print '<hr><pre>';
	var_export( $old_rs );
	print '</pre><hr>';



	foreach ($old_rs as $weight => $col) {
		print 'Untersuche ' . $col->col_name . '<br>';
		if( isset($new_rs[$col->col_name]) ) {
			print 'existiert noch<br>';
			
			if($new_rs[$col->col_name]['title'] == $col->title) {
				print 'Titel ---<br>';
			} else {
				print 'Titel +++<br>';
			}

			if($new_rs[$col->col_name]['type'] == $col->type) {
				print 'type ---<br>';
			} else {
				print 'type +++<br>';
			}

			if($new_rs[$col->col_name]['weight'] == $col->weight) {
				print 'weight ---<br>';
			} else {
				print 'weight +++<br>';
			}

			if(
				isset($new_rs[$col->col_name]['length']) && !isset($col->length) ||
				!isset($new_rs[$col->col_name]['length']) && isset($col->length) ||
				isset($new_rs[$col->col_name]['length'], $col->length) && $new_rs[$col->col_name]['length'] != $col->length
			) {
				print 'length +++<br>';
			} else {
				print 'length ---<br>';
			}

			if(
				isset($new_rs[$col->col_name]['decimal_places']) && !isset($col->decimal_places) ||
				!isset($new_rs[$col->col_name]['decimal_places']) && isset($col->decimal_places) ||
				isset($new_rs[$col->col_name]['decimal_places'], $col->decimal_places) && $new_rs[$col->col_name]['decimal_places'] != $col->decimal_places
			) {
				print 'decimal_places +++<br>';
			} else {
				print 'decimal_places ---<br>';
			}

		} else {
			print 'existiert nicht mehr';
		}

		print '<hr>';
	}

}

?>