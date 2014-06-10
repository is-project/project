<?php
require_once 'errors.inc';



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
		if($project == 0) {
			// initialize project as new one with ID $project = 0
			$this->project = 0;
			$this->parent_project = null;
			$this->name = null;
			$this->description = null;
			
			$this->groups = array();
			// TODO: add system groups
		} else {
			// initialize project with id $project from database
			$project = mysql_query("SELECT * FROM `projects` WHERE `project` = $project");
			if( ($project = mysql_fetch_object($project)) ) { // project found in database and user can access
				$this->project = $project->project;
				$this->parent_project = $project->parent_project;
				$this->name = $project->name;
				$this->description = $project->description;
				$this->record_structure = json_decode($project->record_structure);
				usort($this->record_structure, function($a, $b) {
				    return $a->weight - $b->weight;
				});

				$this->groups = array();
			} else {
				return ERROR_PROJECT_ACCESS_DENIED;
			}
		}
	}

	public function getProjectMetaData() {
		global $current_user;
		if($current_user->access('view_project_metadata', $this->project)) {
			$r = array(
				'project' => $this->project,
				'name' => $this->name,
				'description' => $this->description,
				'parent_project' => $this->parent_project
				);
			return $r;
		} else {
			return ERROR_PROJECT_ACCESS_DENIED;
		}
	}

	public function getProject() {
		return $this->project;
	}

	public function setName($_name) {
		if(1) { // TODO: validate $_name
			$this->name = $_name;
			return true;
		} else {
			return ERROR_INVALID_PROJECT_NAME;
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
			return ERROR_INVALID_PROJECT_DESCRIPTION;
		}
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


	public function saveProject() {
		if($this->project == 0) { // new project, sql INSERT

		} else { // existing project, sql UPDATE

		}
	}
}
?>