<?php

require_once 'inc/init.inc';
require_once 'inc/validations.inc';
require_once 'inc/vo_manageGroups.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';
require_once 'inc/bo_group.class.inc';

global $current_user;
global $current_project;

$layout = new vo_manageGroups();

if(!isset($current_project) || $current_project->getProject() <= 0) $layout->_goto('manageProjects.php');
if(!$current_user->access('manage_groups', $current_project->getProject())) die("Acces Denied");

$layout->setActiveMenuTrail('project-groups');

if(isset($_GET['action'])) {

	switch($_GET['action']) {
		case 'submitAddEditGroupForm':
			$tmp = new bo_group($_POST['group']);
			if($tmp->getSystem()!=0) {
				$layout->showAccesDeniedForm();
				$layout->_print();
				break;
			}
			submitAddEditGroupForm($layout);
			break;

		case 'submitDeleteGroupForm':
			submitDeleteGroupForm($layout);
			break;

		case 'submitAddMemberForm':
		 	submitAddMemberForm($layout);
		 	break;
		
		case 'submitDeleteMemberForm':
			submitDeleteMemberForm($layout);
			break;

		case 'submitAddPermissionsForm':
			submitAddPermissionsForm($layout);
			break;	
		case 'submitDeletePermissionsForm':
			submitDeletePermissionsForm($layout);
			break;
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

if(isset($_GET['location'])&&isset($_GET['group'])){

	switch ($_GET['location']) {
		case 'member':
			$tmp = new bo_group($_GET['group']);
			if($tmp->getSystem()>1) {
				$layout->showAccesDeniedForm();
				$layout->_print();
				break;
			}
			$layout->showEditMembersForm($_GET['group']);
			$layout->_print();
			break;
		case 'permission':
			$tmp = new bo_group($_GET['group']);
			if($tmp->getSystem()==1) {
				$layout->showAccesDeniedForm();
				$layout->_print();
				break;
			}
			$layout->showEditPermissionsForm($_GET['group']);
			$layout->_print();
			break;
	}
}
else if(isset($current_group) && $current_group->getGroup() > 0) {

	
	$metaData = $current_group->getGroupMetaData();

	if( $current_user->access('manage_groups', $current_group->getGroup()) )
		$layout->addJS('settings', array('form-status' => 'edit'));
	$layout->addJS('settings', array('form-values' => array(
		'group' => $metaData['group'],
		'project-name' => $metaData['name'],
		'parent-project' => $metaData['parent_project'],
		'parent-project-name' => 'pp name',
		'project-description' => $metaData['description']
		)));

	$layout->showManageGroupsForm($current_project->getListOfGroups());
	$layout->_print();

} else {	


	$layout->showGroupsOverview( $current_project->getListOfGroups() );
	$layout->_print();

}

function submitAddMemberForm($layout){
	global $current_user;

	if(!isset($_POST['email'])||!isset($_POST['group'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}


	$errors = array();
	$group = new bo_group($_POST['group']);


	switch ($group->addMember($_POST['email'])) {
		case ERROR_INVALID_EMAIL_EMPTY: 
			$errors['name'] = '##Email cannot be empty.##';
			break;
		case ERROR_INVALID_EMAIL_USER_EXISTS:
			$errors['name'] = '##User is allready in this group.##';
			break;
		case ERROR_INVALID_EMAIL_USER_NOT_EXISTING:
			$errors['name'] = '##User does not exist.##';
			break;
		default:

	}


	if(count($errors)) {
		foreach ($errors as $field => $error) {
			$layout->toast($error, 'error');
		}
		$layout->addJS('settings', array('form-errors' => $errors));
		$layout->addJS('settings', array('form-status' => 'edit'));
		$layout->addJS('settings', array('form-values' => array(
			'email' => $_POST['email'],
			'group' => $_POST['group'],
		)));
	} else {
		$layout->toast('##Member was added successfully.##');
	}

}

function submitAddPermissionsForm($layout){
	global $current_user;

	if(!isset($_POST['permission'])||!isset($_POST['group'])) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}


	$errors = array();
	$group = new bo_group($_POST['group']);


	switch ($group->addPermission($_POST['permission'])) {
		case ERROR_INVALID_EMAIL_EMPTY: 
			$errors['name'] = '##Email cannot be empty.##';
			break;
		case ERROR_INVALID_EMAIL_USER_EXISTS:
			$errors['name'] = '##User is allready in this group.##';
			break;
		case ERROR_INVALID_EMAIL_USER_NOT_EXISTING:
			$errors['name'] = '##User does not exist.##';
			break;
		default:

	}


	if(count($errors)) {
		foreach ($errors as $field => $error) {
			$layout->toast($error, 'error');
		}
		$layout->addJS('settings', array('form-errors' => $errors));
		$layout->addJS('settings', array('form-status' => 'edit'));
		$layout->addJS('settings', array('form-values' => array(
			'permission' => $_POST['permission'],
			'group' => $_POST['group'],
		)));
	} else {
		$layout->toast('##Permission was added successfully.##');
	}

}

function submitAddEditGroupForm($layout) {
	global $current_user;
	if(!isset($_POST['group-name'])) {
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
				'group-name' => $_POST['group-name'],
				'group' => $_POST['group'],
				)));
		} else {
			$group->saveGroup();
			$layout->toast('##Group was edited successfully.##');
		}

	} else { // add
		$errors = array();

		$group = new bo_group(0);

		// validate group name

		$trr = $group->setName($_POST['group-name']);

		if ($trr!=1){
			switch($trr) {
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
			$layout->addJS('settings', array('form-status' => 'add'));
			$layout->addJS('settings', array('form-values' => array(
				'project' => $_POST['project'],
				'group-name' => $_POST['group-name'],
				'group' => $_POST['group'],
				)));
		} else {
			$group->saveGroup();
			$layout->toast('##Group was added successfully.##');
		}
	}
}

function submitDeleteGroupForm($layout){
	global $current_user;
	$errors = array();
	$rows = json_decode($_POST['rows']);

	if(!isset($rows)) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}



	if(!is_array($rows) || !count($rows)) $errors = array(ERROR_GROUP_ACCESS_DENIED);

	foreach ($rows as $c) {
			$groupObj = new bo_group($c);
			$trr = $groupObj->deleteGroup(); 
			if ($trr!=1){
				$errors[] = '##Access Denied.##';
			} 
	}
	if(count($errors)) {
		$layout->toast('##Access Denied.##', 'error');
	} else {
		$layout->toast('##Group(s) was/were deleted successfully.##');
	}
}

function submitDeleteMemberForm($layout){
	global $current_user;

	$errors = array();
	$rows = json_decode($_POST['rows']);
	$group = $_POST['group'];

	if(!isset($rows)) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	if(!is_array($rows) || !count($rows)) $errors = array(ERROR_GROUP_ACCESS_DENIED);

	$groupObj = new bo_group($group);

	foreach ($rows as $c) {
		$trr = $groupObj->deleteMember($c); 
		if ($trr!=1){
			$errors[] = '##Access Denied.##';
		} 
	}

	if(count($errors)) {		
		$layout->toast('##Access Denied.##','error');
	} else {
		$layout->toast('##Member(s) was/were deleted successfully.##');
	}
}
function submitDeletePermissionsForm($layout){
	global $current_user;

	$errors = array();
	$rows = json_decode($_POST['rows']);
	$group = $_POST['group'];

	if(!isset($rows)) {
		$layout->toast('##Form Error##', 'error');
		return -1;
	}

	if(!is_array($rows) || !count($rows)) $errors = array(ERROR_GROUP_ACCESS_DENIED);

	$groupObj = new bo_group($group);

	foreach ($rows as $c) {
		$trr = $groupObj->deletePermission($c); 

		if ($trr!=1){
			$errors[] = '##Access Denied.##';
		} 
	}

	if(count($errors)) {		
		$layout->toast('##Access Denied.##','error');
	} else {
		$layout->toast('##Permission(s) was/were deleted successfully.##');
	}

}


?>