<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageProjects.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageGroups();


if(isset($_GET['action'])) {

	switch($_GET['action']) {
		case 'submitAddEditGroupForm':
			submitAddEditGroupForm($layout);
			break;

		case 'deleteGroup':
			deleteProject($layout);
			break;

		// case 'submitEditGroupForm':
		// 	submitDeleteProjectForm($layout);
		// 	break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

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

			$errors['project'] = '##Access denied ##';

		} else {

			// validate project name
			switch( $project->setName($_POST['project-name']) ) {
				case ERROR_INVALID_PROJECT_NAME_EMPTY: 
					$errors['project-name'] = '##Project Name cannot be empty.##';
					break;
				case ERROR_INVALID_PROJECT_NAME_TO_LONG: 
					$errors['project-name'] = '##Project Name cannot be longer than 45 characters.##';
					break;
				case ERROR_PROJECT_ACCESS_DENIED: 
					$errors['project-name'] = '##Access Denied.##';
					break;
			}

			// validate project description
			switch( $project->setDescription($_POST['project-description']) ) {
				case ERROR_PROJECT_ACCESS_DENIED: 
					$errors['project-description'] = '##Access Denied.##';
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
				'project' => $_POST['project'],
				'project-name' => $_POST['project-name'],
				'parent-project' => $_POST['parent-project'],
				'parent-project-name' => $_POST['parent-project-name'],
				'project-description' => $_POST['project-description']
				)));
		} else {
			$project->saveProject();
			$layout->toast('##Project was edited successfully.##');
			global $current_project;
			if( isset($current_project) && $current_project->getProject() == $project->getProject() )
				$current_project = new bo_project( $current_project->getProject() );
		}

	} else { // add
		$errors = array();

		$project = new bo_project();

		// validate parent project
		switch( $project->setParentProject($_POST['parent-project']) ) {
			case ERROR_NO_VALID_INT:
				$errors['parent-project'] = '##Parent-project is not a valid integer.##';
				break;
			case ERROR_PROJECT_EDIT_PARENT:
			case ERROR_PROJECT_ACCESS_DENIED: 
				$errors['parent-project'] = '##Access Denied.##';
				break;
		}

		// validate project name
		switch( $project->setName($_POST['project-name']) ) {
			case ERROR_INVALID_PROJECT_NAME_EMPTY: 
				$errors['project-name'] = '##Project Name cannot be empty.##';
				break;
			case ERROR_INVALID_PROJECT_NAME_TO_LONG: 
				$errors['project-name'] = '##Project Name cannot be longer than 45 characters.##';
				break;
			case ERROR_PROJECT_ACCESS_DENIED: 
				$errors['project-name'] = '##Access Denied.##';
				break;
		}

		// validate project description
		switch( $project->setDescription($_POST['project-description']) ) {
			case ERROR_PROJECT_ACCESS_DENIED: 
				$errors['project-description'] = '##Access Denied.##';
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
				'project-name' => $_POST['project-name'],
				'parent-project' => $_POST['parent-project'],
				'parent-project-name' => $_POST['parent-project-name'],
				'project-description' => $_POST['project-description']
				)));
		} else {
			$project->saveProject();
			$layout->toast('##Project was added successfully.##');
		}
	}
}



?>