<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageProjects.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageProjects();


if(isset($_GET['action'])) {

	switch($_GET['action']) {
		case 'submitAddEditProjectForm':
			submitAddEditProjectForm($layout);
			break;

		case 'deleteProject':
			deleteProject($layout);
			break;

		case 'submitDeleteProjectForm':
			submitDeleteProjectForm($layout);
			break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

if(isset($current_project)) {

	$layout->setAreaContent('menu_project', 'active');
	
	$metaData = $current_project->getProjectMetaData();

	if( $current_user->access('edit_project_meta_data', $current_project->getProject()) )
		$layout->addJS('settings', array('form-status' => 'edit'));
	$layout->addJS('settings', array('form-values' => array(
		'project' => $metaData['project'],
		'project-name' => $metaData['name'],
		'parent-project' => $metaData['parent_project'],
		'parent-project-name' => 'pp name',
		'project-description' => $metaData['description']
		)));

	$layout->showProjectOverview( );
	$layout->_print();

} else {	

	$layout->setAreaContent('menu_overview', 'active');
	$layout->setAreaContent('menu_project', 'disabled');
	$layout->setAreaContent('menu_user', 'disabled');
	$layout->setAreaContent('menu_records', 'disabled');
	$layout->setAreaContent('menu_collections', 'disabled');
	$layout->setAreaContent('menu_register', 'disabled');

	$projects = $current_user->getListOfProjects('view_project_metadata', true);
	$layout->showProjectsOverview( _parseProjectsTree($projects) );
	$layout->_print();

}

function _parseProjectsTree($projects) {
	global $current_user;
	$tmp = array();
	foreach ($projects as $project => $children) {
		$p = new bo_project($project);
		if(($tmp[$project] = $p->getProjectMetaData()) < 0) _die('ERROR#'.$tmp[$project]);
		$tmp[$project]['edit_access'] = $current_user->access('edit_project_metadata', $p->getProject()) ? '1' : '0';
		$tmp[$project]['create_access'] = $current_user->access('create_child_project', $p->getProject()) ? '1' : '0';
		$tmp[$project]['delete_access'] = ($p->getParentProject() == 0) ? '0' : ($current_user->access('delete_child_project', $p->getParentProject()) ? '1' : '0');
		$tmp[$project]['children'] = _parseProjectsTree($children);
	}
	return $tmp;
}

function submitAddEditProjectForm($layout) {
	global $current_user;
	if(!isset($_POST['project'], $_POST['parent-project'], $_POST['project-name'], $_POST['project-description'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}
	if(valid_int($_POST['project'])) { // edit

		$project = new bo_project($_POST['project']);

		$errors = array();
		if($project->getProject() <= 0) {

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

function deleteProject($layout) {
	global $current_user;
	$project = new bo_project($_GET['project']);
	if($project->getProject() <= 0 || !$current_user->access('delete_child_project', $project->getParentProject())) {
		$layout->toast('##Access Denied##', 'error');
	} else {
		$project_list = $project->getListOfChildProjects(true);
		$projects = array();
		foreach ($project_list as $_p) {
			$tmp = new bo_project($_p);
			$projects[] = $tmp->getProjectMetaData();
		}

		$layout->showProjectDeleteForm($project->getProjectMetaData(), $projects);
	}
}

function submitDeleteProjectForm($layout) {
	$project = new bo_project($_POST['project']);
	if($project->getProject() <= 0) $layout->toast('##Access Denied##', 'error');
	else {
		switch ($project->deleteProject()) {
			case ERROR_PROJECT_ACCESS_DENIED:
				$layout->toast('##Access Denied##', 'error');
				break;
			
			default:
				$layout->toast('##Project was deleted successfully.##');
				break;
		}
	}
}

?>