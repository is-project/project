<?php

class bo_group {
	/* properties */

	private $group;
	private $project;
	private $name;
	private $description;
	private $member;
	private $permission;

	/* methods */

	function __construct($group = 0) {
		print 'Ich wurde angelegt, bin Gruppe #'.$group;

		if($group == 0) {
			// init empty group
			$this->group = 0;
			$this->project = null;
		} else {
			// init existing group from DB

			#sql_query('SELECT FROm GROUP WHERE id = group ....')

			$this->group = $group;
			$this->project = rand(0,100);
			$this->name = 'Gruppe #'.$group;
			$this->description = 'DESC Gruppe #'.$group;

			$this->member = array();
			for($i = rand(0,7); $i < 7; $i++)
				$this->member[] = rand(1,100);

			#sql_query('SELECT FROM * link_groups_permissions WHERE id = group ....')

			$this->permission = array();
			for ($i = rand(1,7); $i < 7; $i++)
				$this->permission[] = rand(1,100);
		}
	}

	public function getGroupMetaData() {
		$r = array();
		$r['name'] = $this->name;
		$r['description'] = $this->description;
		$r['group'] = $this->group;

		return $r;
	}

	public function getGroup() {
		global $current_user;
		if(!$current_user->access('view_project_metadata', $this->project)) return ERROR_PROJECT_ACCESS_DENIED;
		
		return $this->group;
	}

	public function getPermissions() {
		return $this->permission;
	}

	public function getMembers() {
		return $this->member;
	}

	public function saveGroup() {
		#sql_query('UPDATE groups WHERE group = $this->group')

		return TRUE;
	}

	public function setName($_name){
		$this->name = $_name;

		return TRUE;
	}

	public function setDescription($_description){
		$this->description = $_description;

		return TRUE;
	}

	public function setMembers($_member){
		$this->member = $_member;

		return TRUE;
	}

	public function setPermissions($_permission){
		$this->permission = $_permission;

		return TRUE;
	}

	public function deletGroup(){
		#sql_query('DELETE FROM group WHERE group = this->group')

		return TRUE;
	}
}

?>