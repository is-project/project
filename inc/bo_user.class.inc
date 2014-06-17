<?php
require_once 'errors.inc';
class bo_user
{
	/* properties */
	private $user;
	private $name;
	private $email;
	private $password;
	private $description;
	private $valid_until;
	private $groups;

	/* methods */

	// construct
	function __construct($user = 0) {
		if($user != 0) {

			// initialize user with id $user from database
			$result = mysql_query("SELECT `user`, `name`, `email`, `description`, `valid_until` FROM `users` WHERE `user` = $user");
			if(mysql_num_rows($result) == 1) {

				$result = mysql_fetch_object($result);

				$this->user = $result->user;
				$this->name = $result->name;
				$this->email = $result->email;
				$this->password = null;
				$this->description = $result->description;
				$this->valid_until = $result->valid_until;
				
				$this->groups = array();
				$groups = mysql_query("SELECT `group` FROM `link_users_groups` WHERE `user` = {$this->user}");
				while($group = mysql_fetch_object($groups))
					$this->groups[] = $group->group;

			} else $user = 0;
			
		}

		if($user == 0) {
			// initialize user as guest with ID $user = 0
			$this->user = 0;
			$this->name = 'guest';
			$this->email = null;
			$this->password = null;
			$this->description = null;
			$this->valid_until = null;
			
			$this->groups = array();
		}

		// TODO: add system groups
		if(0) // TODO: check if user comes from tu intranet
			$query = "SELECT `group` FROM `groups` WHERE `system` = 2 OR `system` = 3;";
		else
			$query = "SELECT `group` FROM `groups` WHERE `system` = 2;";

		$result = mysql_query($query);
		while ($group = mysql_fetch_object($result))
			$this->groups[] = $group->group;

	}

	public function getUser() {
		return $this->user;
	}

	public function setName($_name) {
		if(1) { // TODO: validate $_name
			$this->name = $_name;
			return true;
		} else {
			return ERROR_INVALID_USER_NAME;
		}
	}

	public function getName() {
		return $this->name;
	}
	
	public function setDescription($_description) {
		if(1) { // TODO: validate $_description
			$this->description = $_description;
			return true;
		} else {
			return ERROR_INVALID_USER_DESCRIPTION;
		}
	}

	public function getDescription() {
		return $this->name;
	}

	public function setEMail($_email) {
		if(1) { // TODO: validate $_email
			$this->email = $_email;
			return true;
		} else {
			return ERROR_INVALID_USER_EMAIL;
		}
	}

	public function getEMail() {
		return $this->name;
	}

	public function setPassword($_password) {
		if(1) { // TODO: validate $_password
			$this->password = $_password;
			return true;
		} else {
			return ERROR_INVALID_USER_PASSWORD;
		}
	}

	public function getListOfProjects($permissions, $get_as_tree = false) {
	    $projects = array();
	    $tree = array();

	    $result = mysql_query('SELECT `project`, `parent_project` FROM `projects` ORDER BY `name`');
	    while($project = mysql_fetch_object($result)) {
	    	if( $this->access($permissions, $project->project) ) { // TODO: add permissions
				$projects[$project->project] = $project->parent_project;
				$tree[$project->project] = array();
	    	}

	    }

	    if($get_as_tree) {
		    $levels = array();
		    while(count($projects)) {
		    	$level = array();
		    	foreach ($projects as $project => $parent_project) {
		    		if(!in_array($parent_project,array_keys($projects)))
		    			$level[$project] = $parent_project;
		    	}
		    	foreach ($level as $project => $parent_project)
		    		unset($projects[$project]);
		    	$levels[] = $level;
		    }

		    for ($i=count($levels)-1; $i >= 0 ; $i--) { 
		    	foreach ($levels[$i] as $project => $parent_project) {
		    		if($parent_project != 0 && isset($tree[$parent_project])) {
		    			$tree[$parent_project][$project] = $tree[$project];
		    			unset($tree[$project]);
		    		}
		    	}
		    }
		}

	    return $get_as_tree ? $tree : array_keys($tree);
	}

	public function getListOfCollections($project) {
	    $collections = array();

	    // TODO: add all collection IDs from this user in the given project to the $collections array, 
	    $collections[] = 4;
	    $collections[] = 7;

	    return $collections;
	}

	public function access($permissions, $project) {
		// print $permissions . ', ' . $project . '<br>';
		if(!valid_int($project)) return FALSE;
		if($project == 0) return TRUE;

		if(is_array($permissions))
			$permissions = implode('\', \'', $permissions);
		
		$query = "SELECT `group` FROM `link_groups_permissions` WHERE `permission` IN(SELECT `permission` FROM `permissions` WHERE `name` IN('$permissions')) AND `group` IN ( SELECT `group` FROM `groups` WHERE `project` = $project ) GROUP BY `group`;";

		$groups = mysql_query($query);
		while ($group = mysql_fetch_object($groups)) {
			if(in_array($group->group, $this->groups))
				return TRUE;
		}
		
		$parent_project = mysql_fetch_object(mysql_query("SELECT `parent_project` FROM `projects` WHERE `project` = $project;"));
		return ($parent_project && $parent_project->parent_project) ? $this->access($permissions, $parent_project->parent_project) : FALSE;
		
	}

	public function saveUser() {
		if($this->user == 0) { // new user, sql INSERT

		} else { // existing user, sql UPDATE

		}
	}
}
?>