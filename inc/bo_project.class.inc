<?php
require_once 'errors.inc';
require_once 'validations.inc';



class bo_project
{
	/* properties */
	private $project;
	private $parent_project;
	private $name;
	private $description;
	private $groups;
	private $record_structure;

	/* methods */

	// construct
	function __construct($project = 0) {
		if(!valid_int($project)) $project = 0;
		if($project != 0) {

			// initialize project with id $project from database
			$_project = mysql_query("SELECT * FROM `projects` WHERE `project` = $project");
			if( ($_project = mysql_fetch_object($_project)) ) { // project found in database
				$this->project = $_project->project;
				$this->parent_project = $_project->parent_project;
				$this->name = $_project->name;
				$this->description = $_project->description;
				$this->record_structure = json_decode($_project->record_structure);
				usort($this->record_structure, function($a, $b) {
				    return $a->weight - $b->weight;
				});

				$this->groups = array();
				// TODO: add project groups
			} else {
				$project = 0;
			}

		} 

		if($project == 0) {	

			// initialize project as new one with ID $project = 0
			$this->project = 0;
			$this->parent_project = null;
			$this->name = null;
			$this->description = null;
			// $this->record_structure = '[{"col_name":"record","title":"Record","type":"int","weight":0},{"col_name":"deleted","title":"Deleted","type":"timestamp","weight":1},{"col_name":"deleted_by","title":"Deleted By","type":"int","weight":2},{"col_name":"created","title":"Created","type":"timestamp","weight":3},{"col_name":"created_by","title":"Created By","type":"int","weight":4}]';
			$this->record_structure = '[]';
			
			$this->groups = array();

		}
	}

	public function getProjectMetaData() {
		global $current_user;
		if(!$current_user->access('view_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;
		
		$r = array(
			'project' => $this->project,
			'name' => $this->name,
			'description' => $this->description,
			'parent_project' => $this->getParentProject()
			);
		return $r;
	}

	public function getProject() {
		global $current_user;
		if(!$current_user->access('view_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;
		
		return $this->project;
	}

	public function setName($_name) {
		global $current_user;
		if(!$current_user->access('edit_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;
		if(!strlen($_name)) return ERROR_INVALID_PROJECT_NAME_EMPTY;
		if(strlen($_name) > 45) return ERROR_INVALID_PROJECT_NAME_TO_LONG;

		$this->name = $_name;
		return 1;
	}

	public function getName() {
		global $current_user;
		if(!$current_user->access('view_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;

		return $this->name;
	}

	public function setDescription($_description) {
		global $current_user;
		if(!$current_user->access('edit_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;
			
		if(strlen($_description)) $this->description = $_description;
		else $this->description = null;
		return 1;
	}

	public function getParentProject() {
		global $current_user;
		if(!$current_user->access('view_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;
		if(!$current_user->access('view_project_metadata', $this->parent_project)) return -1;
		
		return $this->parent_project;
	}

	public function setParentProject($_parent) {
		global $current_user;
		if( !valid_int($_parent) ) return ERROR_NO_VALID_INT;
		if( $this->project != 0 ) return ERROR_PROJECT_EDIT_PARENT;
		if(!$current_user->access('create_child_project', $_parent)) return ERROR_PROJECT_ACCESS_DENIED;

		$this->parent_project = $_parent;
		return 1;
	}

	public function getRecordStructure() {

		return $this->record_structure;

		// $result = mysql_query("
		// 	SELECT COLUMN_NAME, DATA_TYPE, COLUMN_TYPE, COLUMN_TYPE, COLUMN_COMMENT
		// 	FROM INFORMATION_SCHEMA.COLUMNS
		// 	WHERE table_name =  'data_project_1'");
		// if($result) {
		// } else {
		// 	return ERROR_PROJECT_GET_RECORD_STRUCTURE_SQL_ERROR;
		// }
	}

	public function setRecordStructure($new_rs) {

		$old_rs = $this->record_structure;

		print '<hr><pre>';
		var_export( $new_rs );
		print '</pre><hr>';

		print '<hr><pre>';
		var_export( $old_rs );
		print '</pre><hr>';

		if( !is_array($new_rs) ) return ERROR_PROJECT_EDIT_RECORD_STRUCTURE;
		// var_export($new_rs); return ERROR_PROJECT_EDIT_RECORD_STRUCTURE;
		foreach ($new_rs as $weight => $col) {
			if(array_keys($col) != array ('col_name', 'title', 'type', 'length', 'decimal_places', 'weight') ) return ERROR_PROJECT_EDIT_RECORD_STRUCTURE;

			print 'Validate col_name<br>';
			if(!is_string($col['col_name']) && !valid_int($col['col_name'])) {print '<hr>';var_export($col); return ERROR_PROJECT_EDIT_RECORD_STRUCTURE;}

			print 'Validate title<br>';
			if(!is_string($col['title']) || $col['title'] == '') {print '<hr>';var_export($col); return ERROR_PROJECT_EDIT_RECORD_STRUCTURE;}

		}

		print 'DEBUG #1 <br>';

		// foreach ($old_rs as $weight => $col) {
		// 	print 'Untersuche ' . $col->col_name . '<br>';
		// 	if( isset($new_rs[$col->col_name]) ) {
		// 		print 'existiert noch<br>';
				
		// 		if($new_rs[$col->col_name]['title'] == $col->title) {
		// 			print 'Titel ---<br>';
		// 		} else {
		// 			print 'Titel +++<br>';
		// 		}

		// 		if($new_rs[$col->col_name]['type'] == $col->type) {
		// 			print 'type ---<br>';
		// 		} else {
		// 			print 'type +++<br>';
		// 		}

		// 		if($new_rs[$col->col_name]['weight'] == $col->weight) {
		// 			print 'weight ---<br>';
		// 		} else {
		// 			print 'weight +++<br>';
		// 		}

		// 		if(
		// 			isset($new_rs[$col->col_name]['length']) && !isset($col->length) ||
		// 			!isset($new_rs[$col->col_name]['length']) && isset($col->length) ||
		// 			isset($new_rs[$col->col_name]['length'], $col->length) && $new_rs[$col->col_name]['length'] != $col->length
		// 		) {
		// 			print 'length +++<br>';
		// 		} else {
		// 			print 'length ---<br>';
		// 		}

		// 		if(
		// 			isset($new_rs[$col->col_name]['decimal_places']) && !isset($col->decimal_places) ||
		// 			!isset($new_rs[$col->col_name]['decimal_places']) && isset($col->decimal_places) ||
		// 			isset($new_rs[$col->col_name]['decimal_places'], $col->decimal_places) && $new_rs[$col->col_name]['decimal_places'] != $col->decimal_places
		// 		) {
		// 			print 'decimal_places +++<br>';
		// 		} else {
		// 			print 'decimal_places ---<br>';
		// 		}

		// 		unset($new_rs[$col->col_name]);

		// 	} else {
		// 		print 'existiert nicht mehr';
		// 	}

		// 	print '<hr>';
		// }

		// foreach ($new_rs as $key => $value) {
		// 	print 'new_rs[\''.$key.'\'] ist neu.<br>';
		// }

		return ERROR_PROJECT_EDIT_RECORD_STRUCTURE;

	}



	public function getListOfChildProjects($all = false) {
		global $current_user;
		if(!$current_user->access('view_project_metadata',$this->project)) return ERROR_PROJECT_ACCESS_DENIED;

		if($all) {
			$projects = array();
			$children = array();
			$result = mysql_query('SELECT `project`, `parent_project` FROM `projects`');
			while($project = mysql_fetch_object($result)) {
				$projects[$project->project] = $project->parent_project;
			}
			$continue = true;
			while ($continue) {
				$continue = false;
				foreach ($projects as $project => $parent) {
					if($parent == $this->project || in_array($parent, $children)) {
						$children[] = $project;
						unset($projects[$project]);
						$continue = true;
					}
				}
			}

			return $children;
		} else {
			$children = array();
			$result = mysql_query("SELECT `project` FROM `projects` WHERE `parent_project` = {$this->project}");
			while($project = mysql_fetch_object($result))
				$children[] = $project->project;
			return $children;
		}
	}

	public function saveProject() {
		global $current_user;
		if($this->project == 0) { // new project, sql INSERT

			if(!isset($this->parent_project, $this->name)) return ERROR_PROJECT_SAVE_INCOMPLETE;
			$name = mysql_real_escape_string( $this->name );
			$description = isset($this->description) ? "'".mysql_real_escape_string( $this->description )."'" : 'NULL';

			// add project
			mysql_query("INSERT INTO `projects` (`parent_project`, `created_by`, `name`, `description`, `record_structure`) VALUES ({$this->parent_project}, {$current_user->getUser()}, '$name', $description, '{$this->record_structure}')") or _die('Error while saving the project.', mysql_error());
			$this->project = mysql_insert_id();

			// add group "project leader"
			mysql_query("INSERT INTO `groups` (`project`, `name`, `system`) VALUES ({$this->project}, 'Project Leader', 1)");
			$group = mysql_insert_id();

			// add permissions to project leader group
			$query = array();
			$permissions = mysql_query("SELECT `permission` FROM `permissions`");
			while($permission = mysql_fetch_object($permissions))
				$query[] = "($group, {$permission->permission})";
			$query = "INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES ".implode(', ', $query);
			mysql_query($query);

			// add creator ass project leader
			$query = "INSERT INTO `link_users_groups` (`user`, `group`) VALUES ({$current_user->getUser()}, $group)";
			mysql_query($query);

			// add group "guest" & "Guest (TU intranet)"
			mysql_query("INSERT INTO `groups` (`project`, `name`, `system`) VALUES ({$this->project}, 'Guest', 2), ({$this->project}, 'Guest (TU intranet)', 3)");

			// create data table
			mysql_query("	CREATE TABLE IF NOT EXISTS `data_project_{$this->project}` (
							  `entry` int(11) NOT NULL AUTO_INCREMENT,
							  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
							  `created_by` int(11) NOT NULL,
							  `deleted` timestamp NULL DEFAULT NULL,
							  `deleted_by` int(11) DEFAULT NULL,
  							PRIMARY KEY (`entry`)
							) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;");

		} else { // existing project, sql UPDATE
			$name = mysql_real_escape_string( $this->name );
			$description = isset($this->description) ? "'".mysql_real_escape_string( $this->description )."'" : 'NULL';

			mysql_query("UPDATE `projects` SET `name` = '$name', `description` = $description WHERE `project` = {$this->project} LIMIT 1;") or _die('Error while saving the project.', mysql_error());
		}
	}

	public function deleteProject() {
		global $current_user;
		if(!$current_user->access('delete_child_project', $this->getParentProject())) return ERROR_PROJECT_ACCESS_DENIED;
		$projects = $this->getListOfChildProjects();
		foreach ($projects as $project) {
			$project = new bo_project($project);
			$project->deleteProject();
		}

		$groups = array();
		$result = mysql_query("SELECT `group` FROM `groups` WHERE `project` = {$this->project};");
		while($group = mysql_fetch_object($result))
			$groups[] = $group->group;

		mysql_query("DELETE FROM `link_users_groups` WHERE `group` IN(".(implode(", ", $groups)).");");
		mysql_query("DELETE FROM `link_groups_permissions` WHERE `group` IN(".(implode(", ", $groups)).");");
		mysql_query("DELETE FROM `groups` WHERE `project` = {$this->project};");
		mysql_query("DELETE FROM `projects` WHERE `project` = {$this->project};");
		mysql_query("DROP TABLE data_project_{$this->project};");
	}
}
?>