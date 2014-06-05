<?php
require_once 'errors.inc';
class bo_project
{
	/* properties */
	private $project;
	private $parentProject;
	private $name;
	private $description;
	private $groups;

	/* methods */

	// construct
	function __construct($project = null) {
		if(is_null($project)) {
			// initialize project as new one with ID $project = 0
			$this->project = 0;
			$this->parentProject = null;
			$this->name = null;
			$this->description = null;
			
			$this->groups = array();
			// TODO: add system groups
		} else {
			// initialize project with id $project from database
			if(1) { // project found in database and user can access
				$this->project = $project;
				$this->parentProject = null;
				$this->name = 'Project #'.$project.' Name';
				$this->description = 'description of Project number #'.$project;
				$this->groups = array();
				for($i = rand(0,7); $i < 7; $i++)
					$this->groups[] = rand(1,100);
			} else {
				return ERROR_PROJECT_ACCESS_DENIED;
			}
		}
	}

	public function getProjectMetaData() {
		$r = array(
			'project' => $this->project,
			'name' => $this->name,
			'description' => $this->description
			);
		return $r;
	}

	public function setName($_name) {
		if(1) { // TODO: validate $_name
			$this->name = $_name;
			return true;
		} else {
			return ERROR_INVALID_PROJECT_NAME;
		}
	}

	public function setDescription($_description) {
		if(1) { // TODO: validate $_description
			$this->description = $_description;
			return true;
		} else {
			return ERROR_INVALID_PROJECT_DESCRIPTION;
		}
	}

	public function getRecordStructure() {
		$result = mysql_query("
			SELECT COLUMN_NAME, DATA_TYPE, COLUMN_TYPE, COLUMN_TYPE, COLUMN_COMMENT
			FROM INFORMATION_SCHEMA.COLUMNS
			WHERE table_name =  'tab1'");
		if($result) {
			
			while($col = mysql_fetch_object($result)) {
				print '<hr><pre>';
				var_export($col);
				print '</pre><hr>';
			}
		} else {
			return ERROR_PROJECT_GET_RECORD_STRUCTURE_SQL_ERROR;
		}
	}


	public function saveProject() {
		if($this->project == 0) { // new project, sql INSERT

		} else { // existing project, sql UPDATE

		}
	}
}
?>