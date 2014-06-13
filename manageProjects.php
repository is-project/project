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

if(isset($_POST['action'])) {

	switch ($_POST['action']) {
		case 'submitAddEditProjectForm':
			// TODO: addProject
			if(valid_int($_POST['project'])) { // edit
				
				$errors = array();
				// validate user permissions to edit
				if( !$current_user->access('edit_project_metadata', $_POST['project']) )
					$errors['project'] = '##Access denied for project ###'.$_POST['project'];

				// validate project name
				if( !strlen($_POST['project-name']) )
					$errors['project-name'] = '##Project Name cannot be empty.##';
				
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
					$project = $_POST['project'];
					$name = mysql_real_escape_string( $_POST['project-name'] );
					$description = mysql_real_escape_string( $_POST['project-description'] );

					mysql_query("UPDATE `projects` SET `name` = '$name', `description` = '$description' WHERE `project` = $project LIMIT 1;") or _die('janek', mysql_error());
					
					$layout->toast('##Project was edited successfully.##');
					
				}

			} else { // add
				$errors = array();

				// validate user permissions to add sub-project
				if(!valid_int($_POST['parent-project']))
					$errors['parent-project'] = '##Parent project has to a valid integer.##';
				elseif( !$current_user->access('edit_project_metadata', $_POST['parent-project']) )
					$errors['parent-project'] = '##Access denied. You cannot create a sub-project for project ###'.$_POST['parent-project'];

				// validate project name
				if( !strlen($_POST['project-name']) )
					$errors['project-name'] = '##Project Name cannot be empty.##';
				
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
					$parent_project = $_POST['parent-project'];
					$name = mysql_real_escape_string( $_POST['project-name'] );
					$description = mysql_real_escape_string( $_POST['project-description'] );
					$default_record_structure = '[{"col_name":"record","title":"Record","type":"int","weight":0},{"col_name":"deleted","title":"Deleted","type":"timestamp","weight":1},{"col_name":"deleted_by","title":"Deleted By","type":"int","weight":2},{"col_name":"created","title":"Created","type":"timestamp","weight":3},{"col_name":"created_by","title":"Created By","type":"int","weight":4}]';

					mysql_query("INSERT INTO `projects` (`parent_project`, `name`, `description`, `record_structure`) VALUES ($parent_project, '$name', '$description', '$default_record_structure')") or _die('janek', mysql_error());
					
					$layout->toast('##Project was added successfully.##');
					
				}
			}
			
			// $layout->toast('Project '.$_POST['project'].' no int');
			// $name = $_POST['project-name'];
			// mysql_query("INSERT INTO `projects` (`parent_project`, `name`, `description`, `record_structure`) VALUES (2, '$name', 'desc', '[[]]')");
			break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

$projects = $current_user->getListOfProjects('view_project_metadata', true);
$layout->showProjectOverview( _parseProjectsTree($projects) );

function _parseProjectsTree($projects) {
	global $current_user;
	$tmp = array();
	foreach ($projects as $project => $children) {
		$p = new bo_project($project);
		if(($tmp[$project] = $p->getProjectMetaData()) < 0) _die('ERROR#'.$tmp[$project]);
		$tmp[$project]['edit_access'] = $current_user->access('edit_project_metadata', $p->getProject()) ? '1' : '0';
		$tmp[$project]['create_access'] = $current_user->access('create_child_project', $p->getProject()) ? '1' : '0';
		$tmp[$project]['children'] = _parseProjectsTree($children);
	}
	return $tmp;
}

?>