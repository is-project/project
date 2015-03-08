<?php

class bo_group {
	/* properties */

	private $group;
	private $project;
	private $name;
	private $system;
	private $member;
	private $permission;

	/* methods */

	function __construct($group) {
		if(!valid_int($group)) $group = 0;
		
		if($group != 0) {
			$this->member = array();
			$this->permission = array();
			// initialize group with id $group from database
			$_group = mysql_query("SELECT * FROM `groups` WHERE `group` = $group");
			if( ($_group = mysql_fetch_object($_group)) ) { // group found in database
				$this->group = $_group->group;
				$this->project = $_group->project;
				$this->name = $_group->name;
				$this->system = $_group->system;
				$_member = mysql_query("SELECT `user` FROM `link_users_groups` WHERE `group` = $group");
				while($result = mysql_fetch_object($_member)) {
	    			$this->member[] = $result;
			    }

			    $_permission = mysql_query("SELECT `permission` FROM `link_groups_permissions` WHERE `group` = $group");
				while($result = mysql_fetch_object($_permission)) {
	    			$this->permission[] = $result;
			    }
				
			} else {
				$group = 0;
			}

		} 

		if($group == 0) {	
			global $current_project;
			// initialize group as new one with ID $group = 0
			$this->group = 0;
			$this->project = $current_project->getProject();
			$this->name = null;
			$this->system = 0;
			
			$this->member = array();
			$this->permission = array();

		}
	}

	public function getGroupMetaData() {
		global $current_user;
		global $current_project;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;


		$r = array();
		$r['group'] = $this->group;
		$r['project'] = $this->project;
		$r['name'] = $this->name;
		$r['system'] = $this->system;
		$r['member'] = array();
		$r['member'] = $this->member;
		$r['permission'] = array();
		$r['permission'] = $this->permission;

		return $r;
	}

	public function getGroup() {
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;
		
		return $this->group;
	}

	public function getPermissions() {
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		return $this->permission;
	}

	public function getMembers() {
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		return $this->member;
	}

	public function saveGroup() {
		#sql_query('UPDATE groups WHERE group = $this->group')
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		if($this->group == 0) { // new group, sql INSERT

			if(!isset($this->project, $this->name)) return ERROR_PROJECT_SAVE_INCOMPLETE;
			$name = mysql_real_escape_string( $this->name );

			// add group
			mysql_query("INSERT INTO `groups` (`project`, `name`, `system`) VALUES ({$this->project}, '$name', '$system')") or _die('Error while saving the project.', mysql_error());
			$this->group = mysql_insert_id();

			// add permissions to group
			foreach ($permission as $_permission) {
				mysql_query("INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES ({$this->group}, '$_permission')") or _die('Error while saving the project.', mysql_error());;
			}
			
			// add users to group
			foreach ($member as $_member) {
				mysql_query("INSERT INTO `link_users_groups` (`group`, `user`) VALUES ({$this->group}, '$_member')") or _die('Error while saving the project.', mysql_error());;
			}

		} else { // existing project, sql UPDATE
			$name = mysql_real_escape_string( $this->name );

			mysql_query("UPDATE `groups` SET `name` = '$name', `system` = $system WHERE `project` = {$this->project} AND `group` = {this->group} LIMIT 1;") or _die('Error while saving the project.', mysql_error());
		}

	}

	public function setName($_name){
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;
		if($_name == "") return ERROR_INVALID_GROUP_NAME_EMPTY;
		
		$this->name = $_name;

		return TRUE;
	}

	public function setMembers($_member){
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		$this->member = $_member;

		return TRUE;
	}

	public function setPermissions($_permission){
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		$this->permission = $_permission;

		return TRUE;
	}

	public function deletGroup(){
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		mysql_query("DELETE FROM `link_users_groups` WHERE `group` = {this->group}");
		mysql_query("DELETE FROM `link_groups_permissions` WHERE `group` = {this->group}");
		mysql_query("DELETE FROM `groups` WHERE `group` = {this->group}");

		#sql_query('DELETE FROM group WHERE group = this->group')

		return TRUE;
	}
}

?>