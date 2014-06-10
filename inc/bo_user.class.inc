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
			// TODO: add system groups
			if(1/* user IP in TU ip pool */)
				$this->userGroups[] = 1; // ID of the group "tu user"			
		}
	}

	public function setName($_name) {
		if(1) { // TODO: validate $_name
			$this->name = $_name;
			return true;
		} else {
			return ERROR_INVALID_USER_NAME;
		}
	}
	
	public function setDescription($_description) {
		if(1) { // TODO: validate $_description
			$this->description = $_description;
			return true;
		} else {
			return ERROR_INVALID_USER_DESCRIPTION;
		}
	}

	public function setEMail($_email) {
		if(1) { // TODO: validate $_email
			$this->email = $_email;
			return true;
		} else {
			return ERROR_INVALID_USER_EMAIL;
		}
	}

	public function setPassword($_password) {
		if(1) { // TODO: validate $_password
			$this->password = $_password;
			return true;
		} else {
			return ERROR_INVALID_USER_PASSWORD;
		}
	}

	public function getListOfProjects() {
	    $projects = array();

	    // TODO: add all project IDs to the $projects array, when the user can access them in any way. Example:
	    $projects[] = 4;
	    $projects[] = 7;

	    return $projects;
	}

	public function getListOfCollections($project) {
	    $collections = array();

	    // TODO: add all collection IDs from this user in the given project to the $collections array, 
	    $collections[] = 4;
	    $collections[] = 7;

	    return $collections;
	}

	public function saveUser() {
		if($this->user == 0) { // new user, sql INSERT

		} else { // existing user, sql UPDATE

		}
	}
}
?>