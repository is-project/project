<?php

require_once 'inc/init.inc';
require_once 'inc/vo_manageProjects.class.inc';
require_once 'inc/bo_user.class.inc';
require_once 'inc/bo_project.class.inc';

$layout = new vo_manageProjects();
global $current_user;

print '<hr><pre>';
var_export($current_user);
print '</pre><hr>';

print '<hr><pre>';
var_export($current_user->getListOfProjects(true));
print '</pre><hr>';

if(isset($_GET['action'])) {

	switch ($_GET['action']) {
		case 'submitAddProjectForm':
			// TODO: addProject
			$layout->toast('Project was added successfully');
			break;
		
		default:
			$layout->toast('Wrong Action', 'error');
			break;
	}

}

$projects = $current_user->getListOfProjects(true);

$tmp = _parseProjectsTree($projects);
// foreach ($projects as $project => $children) {
// 	$p = new bo_project($project);
// 	$tmp[$project] = $p->getProjectMetaData();
// 	$tmp[$project]['children'] = $children;
// }
// print '<hr><pre>';
// var_export($tmp);
// print '</pre><hr>';
$layout->showProjectOverview($tmp);

function _parseProjectsTree($projects) {
	$tmp = array();
	foreach ($projects as $project => $children) {
		$p = new bo_project($project);
		$tmp[$project] = $p->getProjectMetaData();
		$tmp[$project]['children'] = _parseProjectsTree($children);
	}
	return $tmp;
}

?>