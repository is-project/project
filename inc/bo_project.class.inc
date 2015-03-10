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
				$_group = mysql_query("SELECT `group` FROM `groups` WHERE `project` = $project");
				while($result = mysql_fetch_object($_group)) {
	    			$this->groups[] = $result;
			    }
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
			$this->record_structure = array();
			
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

	}

	public function addEditRecordStructureColoumn($new_rs) {
		global $current_user;

		$errors = array();

		if(!$current_user->access('edit_record_structure', $this->project)) return array('ERROR_PROJECT_ACCESS_DENIED');
		

		if(!is_string($new_rs['title']))
			$errors[] = ERROR_RECORD_STRUCTURE_TITLE_NO_STRING;

		if($new_rs['title'] == '')
			$errors[] = ERROR_RECORD_STRUCTURE_TITLE_EMPTY;

		if( !in_array($new_rs['type'], array('int','double','timestamp','boolean','text') ) )
			$errors[] = ERROR_RECORD_STRUCTURE_TYPE_INVALID;

		if( $new_rs['type'] == 'text' && !valid_int($new_rs['length']) )
			$errors[] = ERROR_RECORD_STRUCTURE_LENGTH_INVALID;

		if( $new_rs['type'] == 'double' && !valid_int($new_rs['dec_places']) )
			$errors[] = ERROR_RECORD_STRUCTURE_DECPLACES_INVALID;

		if( $new_rs['col'] != '' ) {
			$valid = false;
			foreach ($this->record_structure as $c) {
				if($c->col_name == $new_rs['col']) {
					$valid = true;
					break;
				}
			}
			if(!$valid) $errors[] = ERROR_RECORD_STRUCTURE_COL_INVALID;
		}

		if(count($errors)) return $errors;
		
		if($new_rs['col'] == '') { // add
			$new_rs['decimal_places'] = $new_rs['dec_places'];
			unset($new_rs['dec_places']);
			unset($new_rs['col']);
			$new_rs['weight'] = count($this->record_structure);

			$max = 0;
			foreach ($this->record_structure as $c) {
				$tmp = intval( str_replace('param', '', $c->col_name) );
				if($tmp > $max) $max = $tmp;
			}

			$new_rs['col_name'] = 'param'.($max+1);

			switch ($new_rs['type']) {
				case 'int':
					mysql_query("ALTER TABLE `data_project_{$this->project}` ADD `{$new_rs['col_name']}` INT NOT NULL ;");
					break;
				case 'double':
					mysql_query("ALTER TABLE `data_project_{$this->project}` ADD `{$new_rs['col_name']}` DOUBLE NOT NULL ;");
					break;
				case 'timestamp':
					mysql_query("ALTER TABLE `data_project_{$this->project}` ADD `{$new_rs['col_name']}` TIMESTAMP NULL DEFAULT NULL ;");
					break;
				case 'boolean':
					mysql_query("ALTER TABLE `data_project_{$this->project}` ADD `{$new_rs['col_name']}` BOOLEAN NOT NULL ;");
					break;
				case 'text':
					mysql_query("ALTER TABLE `data_project_{$this->project}` ADD `{$new_rs['col_name']}` VARCHAR({$new_rs['length']}) NOT NULL ;");
					break;
			}
			$this->record_structure[] = $new_rs;

			$this->record_structure = json_encode($this->record_structure);
			$this->record_structure = json_decode($this->record_structure);

			mysql_query('UPDATE `projects` SET `record_structure` = \''.mysql_real_escape_string(json_encode($this->record_structure)).'\'WHERE `project` = '.$this->project.' LIMIT 1;') or _die('Error while saving the project.', mysql_error());
		} else { // edit
			$new_rs['col_name'] = $new_rs['col'];
			unset($new_rs['col']);
			
			switch ($new_rs['type']) {
				case 'int':
					mysql_query("ALTER TABLE `data_project_{$this->project}` CHANGE `{$new_rs['col_name']}` `{$new_rs['col_name']}` INT NOT NULL;");
					break;
				case 'double':
					mysql_query("ALTER TABLE `data_project_{$this->project}` CHANGE `{$new_rs['col_name']}` `{$new_rs['col_name']}` DOUBLE NOT NULL;");
					break;
				case 'timestamp':
					mysql_query("ALTER TABLE `data_project_{$this->project}` CHANGE `{$new_rs['col_name']}` `{$new_rs['col_name']}` TIMESTAMP NULL DEFAULT NULL;");
					break;
				case 'boolean':
					mysql_query("ALTER TABLE `data_project_{$this->project}` CHANGE `{$new_rs['col_name']}` `{$new_rs['col_name']}` BOOLEAN NOT NULL;");
					break;
				case 'text':
					mysql_query("ALTER TABLE `data_project_{$this->project}` CHANGE `{$new_rs['col_name']}` `{$new_rs['col_name']}` VARCHAR({$new_rs['length']}) NOT NULL;");
					break;
			}

			foreach ($this->record_structure as $key => $c) {
				if($c->col_name == $new_rs['col_name']) {
					$new_rs['weight'] = $this->record_structure[$key]->weight;
					$this->record_structure[$key] = $new_rs;
					break;
				}
			}

			$this->record_structure = json_encode($this->record_structure);
			$this->record_structure = json_decode($this->record_structure);

			mysql_query('UPDATE `projects` SET `record_structure` = \''.mysql_real_escape_string(json_encode($this->record_structure)).'\'WHERE `project` = '.$this->project.' LIMIT 1;') or _die('Error while saving the project.', mysql_error());

		}
	}

	public function deleteRecordStructureColoumn($cols) {
		global $current_user;
		if(!$current_user->access('edit_record_structure', $this->project)) return array(ERROR_PROJECT_ACCESS_DENIED);

		$cols = json_decode($cols['cols']);

		if(!is_array($cols) || !count($cols)) return array(ERROR_PROJECT_ACCESS_DENIED);

		$del = array();
		foreach ($this->record_structure as $c) {

			if( in_array($c->col_name, $cols) ) {
				$del[] = $c->col_name;
			}
		}

		if( count($cols) != count($del) ) return array(ERROR_PROJECT_ACCESS_DENIED);

		foreach ($del as $c) {
			mysql_query("ALTER TABLE `data_project_{$this->project}` DROP `$c`;");
			foreach ($this->record_structure as $k => $v) {
				if($c == $v->col_name) unset( $this->record_structure[$k] );
			}
		}
		usort($this->record_structure, function($a, $b) {
		    return $a->weight - $b->weight;
		});
		$this->record_structure = json_encode($this->record_structure);
		$this->record_structure = json_decode($this->record_structure);
		mysql_query('UPDATE `projects` SET `record_structure` = \''.mysql_real_escape_string(json_encode($this->record_structure)).'\'WHERE `project` = '.$this->project.' LIMIT 1;') or _die('Error while saving the project.', mysql_error());

	}

	public function orderRecordStructureColoumn($cols) {
		global $current_user;
		if(!$current_user->access('edit_record_structure', $this->project)) return array(ERROR_PROJECT_ACCESS_DENIED);

		$cols = json_decode($cols['order']);

		if(!is_array($cols) || !count($cols)) return array(ERROR_PROJECT_ACCESS_DENIED);

		$tmp = array();
		foreach ($this->record_structure as $c) {

			if( in_array($c->col_name, $cols) ) {
				$tmp[] = $c->col_name;
			}
		}

		if( count($cols) != count($tmp) ) return array(ERROR_PROJECT_ACCESS_DENIED);

		foreach ($cols as $w => $c) {
			foreach ($this->record_structure as $k => $v) {
				if($c == $v->col_name) $this->record_structure[$k]->weight = $w;
			}
		}
		usort($this->record_structure, function($a, $b) {
		    return $a->weight - $b->weight;
		});
		$this->record_structure = json_encode($this->record_structure);
		$this->record_structure = json_decode($this->record_structure);
		mysql_query('UPDATE `projects` SET `record_structure` = \''.mysql_real_escape_string(json_encode($this->record_structure)).'\'WHERE `project` = '.$this->project.' LIMIT 1;') or _die('Error while saving the project.', mysql_error());

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


	public function getListOfGroups(){
		global $current_user;
		if(!$current_user->access('view_project_metadata',$this->project) && !!$current_user->access('manage_groups',$this->project)) return ERROR_PROJECT_ACCESS_DENIED;

		return $this->groups;
	}

	public function saveProject() {
		global $current_user;
		if($this->project == 0) { // new project, sql INSERT

			if(!isset($this->parent_project, $this->name)) return ERROR_PROJECT_SAVE_INCOMPLETE;
			$name = mysql_real_escape_string( $this->name );
			$description = isset($this->description) ? "'".mysql_real_escape_string( $this->description )."'" : 'NULL';

			// add project
			mysql_query("INSERT INTO `projects` (`parent_project`, `created_by`, `name`, `description`, `record_structure`) VALUES ({$this->parent_project}, {$current_user->getUser()}, '$name', $description, '".json_encode($this->record_structure)."')") or _die('Error while saving the project.', mysql_error());
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