<?php
require_once 'errors.inc';

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
					
	    			$this->member[] = $result->user;
			    }

			    $_permission = mysql_query("SELECT `permission` FROM `link_groups_permissions` WHERE `group` = $group");
				while($result = mysql_fetch_object($_permission)) {
	    			$this->permission[] = $result->permission;
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

	public function getName() {
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		return $this->name;
	}

	public function getSystem(){
		global $current_user;
		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		return $this->system;
	}

	public function saveGroup() {
		#sql_query('UPDATE groups WHERE group = $this->group')
		global $current_user;
		global $current_project;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		if($this->group == 0) { // new group, sql INSERT

			if(!isset($this->project, $this->name)) return ERROR_PROJECT_SAVE_INCOMPLETE;
			$name = mysql_real_escape_string( $this->name );

			// add group
			mysql_query("INSERT INTO `groups` (`project`, `name`, `system`) VALUES ({$this->project}, '$name', '$system')") or _die('Error while saving the project.'. mysql_error());
			$this->group = mysql_insert_id();

			// add permissions to group
			foreach ($this->permission as $_permission) {
				mysql_query("INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES ({$this->group}, '$_permission')") or _die('Error while saving the project.'.mysql_error());;
			}
			
			// add users to group
			foreach ($this->member as $_member) {
				mysql_query("INSERT INTO `link_users_groups` (`group`, `user`) VALUES ({$this->group}, '$_member')") or _die('Error while saving the project.'. mysql_error());;
			}


		} else { // existing project, sql UPDATE
			$name = mysql_real_escape_string( $this->name );


			mysql_query("UPDATE `groups` SET `name` = '$name' WHERE `group` = {$this->group}") or _die('Error while saving the project.'. mysql_error());
		}
		$current_project = new bo_project($current_project->getProject());
	}



	public function setName($_name){
		global $current_user;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;
		if($_name == ""){
			return ERROR_INVALID_GROUP_NAME_EMPTY;
		}

		$this->name = $_name;

		return 1;
	}

	public function addMember($_email){
		global $current_user;

		if($this->system >= 2) return ERROR_GROUP_ACCESS_DENIED;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;


		if($_email == "") return ERROR_INVALID_EMAIL_EMPTY;

		$_member = "";
		$_member = mysql_query("SELECT * FROM `users` WHERE `email` = '$_email'");

		if (mysql_num_rows($_member) == 0) return ERROR_INVALID_EMAIL_USER_NOT_EXISTING;

		$_member = mysql_fetch_object($_member);


		foreach ($this->member as $groupMember) {
			if ($_member->user == $groupMember) return ERROR_INVALID_EMAIL_USER_EXISTS;
		}
		$this->member[] = $_member->user;



		mysql_query("INSERT INTO `link_users_groups` (`group`, `user`) VALUES ({$this->group}, '$_member->user')") or _die('Error while saving the group.', mysql_error());

	}

	public function addPermission($_permission){
		global $current_user;

		if($this->system == 1) return ERROR_GROUP_ACCESS_DENIED;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;


		if($_permission == "") return ERROR_INVALID_PERMISSION_EMPTY;

		$perm = "";
		$perm = mysql_query("SELECT * FROM `permissions` WHERE `permission` = $_permission");

		if (mysql_num_rows($perm) == 0) return ERROR_INVALID_PERMISSION_NOT_EXISTING;

		$perm = mysql_fetch_object($perm);


		foreach ($this->permission as $groupPermission) {
			if ($perm->permission == $groupPermission) return ERROR_INVALID_PERMISSION_EXISTS;
		}
		$this->permissions[] = $perm->permission;

		mysql_query("INSERT INTO `link_groups_permissions` (`group`, `permission`) VALUES ({$this->group}, '$perm->permission')") or _die('Error while saving the group.', mysql_error());
	}

	public function deleteGroup(){
		global $current_user;
		global $current_project;

		if($this->system != 0) return ERROR_GROUP_ACCESS_DENIED;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		mysql_query("DELETE FROM `link_users_groups` WHERE `group` = {$this->group}");
		mysql_query("DELETE FROM `link_groups_permissions` WHERE `group` = {$this->group}");
		mysql_query("DELETE FROM `groups` WHERE `group` = {$this->group}");

		$current_project = new bo_project($current_project->getProject());

		return TRUE;
	}

	public function deleteMember($user){
		global $current_user;
		global $current_project;

		if($this->system==1 && count($this->member)==1) return ERROR_GROUP_ACCESS_DENIED;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		mysql_query("DELETE FROM `link_users_groups` WHERE `user` = $user AND `group` = {$this->group}");
		$current_project = new bo_project($current_project->getProject());

		$this->member='';
		$_member = mysql_query("SELECT `user` FROM `link_users_groups` WHERE `group` = {$this->group}");
		while($result = mysql_fetch_object($_member)) {
			$this->member[] = $result->user;
		}

		return TRUE;
	}

	public function deletePermission($permission){
		global $current_user;
		global $current_project;

		if($this->system==1 && count($this->member)==1) return ERROR_GROUP_ACCESS_DENIED;

		if(!$current_user->access('manage_groups', $this->project)) return ERROR_GROUP_ACCESS_DENIED;

		mysql_query("DELETE FROM `link_groups_permissions` WHERE `permission` = $permission AND `group` = {$this->group}");
		$current_project = new bo_project($current_project->getProject());

		$this->permission='';
		$_permission = mysql_query("SELECT `permission` FROM `link_groups_permissions` WHERE `group` = {$this->group}");
		while($result = mysql_fetch_object($_permission)) {
			$this->permission[] = $result->permission;
		}

		return TRUE;
	}
}

?>