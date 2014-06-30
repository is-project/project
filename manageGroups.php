<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageGroups.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';
require_once 'inc/bo_group.class.inc'


global $current_user;
global $current_project;
global $current_group;

$layout = new vo_manageGroups();


if(isset($_GET['action'])) {

	switch($_GET['action']) {
		case 'submitAddEditGroupForm':
			submitAddEditGroupForm($layout);
			break;

		case 'deleteGroup':
			deleteGroup($layout);
			break;

		// case 'submitEditGroupForm':
		// 	submitDeleteProjectForm($layout);
		// 	break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

if(isset($current_group) && $current_group->getGroup() > 0){

	$layout->setActiveMenuTrail('group-details');

	$metaData = current_group->getGroupMetaData();

	if($current_user->acces('edit_group_metadata', $current_group->getGroup()))
		$layout->addJS('settings', array('form-status' => 'edit'));
	$layout->addJS('settings', array('form-values' => array(
		'group' => $metaData['group'],
		'name' => $metaData['name'],
		'project' => $metaData['project'],
		'description' => $metaData['description']
		)))
}

function submitAddEditGroupForm($layout) {
	global $current_user;
	if(!isset($_POST['group'], $_POST['project'], $_POST['name'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}
	if(valid_int($_POST['group'])) { // edit

		$group = new bo_group($_POST['group']);

		$errors = array();
		if($group->getGroup() <= 0) {

			$errors['group'] = '##Access denied ##';

		} else {

			// validate group name
			switch( $group->setName($_POST['group-name']) ) {
				case ERROR_INVALID_GROUP_NAME_EMPTY: 
					$errors['name'] = '##Group Name cannot be empty.##';
					break;
				case ERROR_INVALID_GROUP_NAME_TO_LONG: 
					$errors['name'] = '##Group Name cannot be longer than 45 characters.##';
					break;
				case ERROR_GROUP_ACCESS_DENIED: 
					$errors['name'] = '##Access Denied.##';
					break;
			}	
		}
	

		if(count($errors)) {
			foreach ($errors as $field => $error) {
				$layout->toast($error, 'error');
			}
			$layout->addJS('settings', array('form-errors' => $errors));
			$layout->addJS('settings', array('form-status' => 'edit'));
			$layout->addJS('settings', array('form-values' => array(
				'group' => $_POST['group'],
				'name' => $_POST['name']
				'project' => $_POST['project']
				'description' => $_POST['description']
				)));
		} else {
			$group->saveGroup();
			$layout->toast('##Group was edited successfully.##');
			global $current_group;
			if( isset($current_group) && $current_group->getGroup() == $group->getGroup() )
				$current_group = new bo_group( $current_group->getGroup() );
		}

	} else { // add
		$errors = array();

		$project = new bo_group();


		// validate group name
		switch( $group->setName($_POST['name']) ) {
			case ERROR_INVALID_GROUP_NAME_EMPTY: 
				$errors['name'] = '##Group Name cannot be empty.##';
				break;
			case ERROR_INVALID_GROUP_NAME_TO_LONG: 
				$errors['name'] = '##Group Name cannot be longer than 45 characters.##';
				break;
			case ERROR_GROUP_ACCESS_DENIED: 
				$errors['name'] = '##Access Denied.##';
				break;
		}

		// validate group description
		switch( $group->setDescription($_POST['description']) ) {
			case ERROR_PROJECT_ACCESS_DENIED: 
				$errors['description'] = '##Access Denied.##';
				break;
		}

		if(count($errors)) {
			foreach ($errors as $field => $error) {
				$layout->toast($error, 'error');
			}
			$layout->addJS('settings', array('form-errors' => $errors));
			$layout->addJS('settings', array('form-status' => 'add'));
			$layout->addJS('settings', array('form-values' => array(
				'project' => $_POST['project'],
				'name' => $_POST['name'],
				'group' => $_POST['group'],
				'description' => $_POST['description']
				)));
		} else {
			$group->saveGroup();
			$layout->toast('##Group was added successfully.##');
		}
	}
}



?>