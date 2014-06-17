<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageProjects.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

global $current_user;
$layout = new vo_manageProjects();

print '<hr><pre>';
var_export($current_user);
print '</pre><hr>';

if(isset($_GET['action'])) {

	switch ($_GET['action']) {
		case 'submitAddEditProjectForm':
				submitAddEditProjectForm($layout);
			break;

		case 'deleteProject':
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
			break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

$projects = $current_user->getListOfProjects('view_project_metadata', true);
$layout->showProjectOverview( _parseProjectsTree($projects) );
$layout->_print();

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
		// validate user permissions to edit
		if($project->getProject() == 0 || !$current_user->access('edit_project_metadata', $project->getProject()) ) {

			$errors['project'] = '##Access denied for project ###'.$_POST['project'];

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