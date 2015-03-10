<?php

require_once 'errors.inc';

class bo_collection {
	/* properties */

	private $collection;
	private $project;
	private $name;
	private $description;
	private $user;
	private $records;

	/* methods */

	function __construct($collection = 0, $project = 0, $virtual = false) {
		global $current_user;

		if($collection == 0) {
			if($project  == 0) {
				return ERROR_COLLECTION_ACCESS_DENIED;
			} else {
				if($virtual) {
					// init virtual collection with all records from the given project
					// TODO: check permission for user -> project
					$this->collection = -1;
					$this->project = $project;
					$this->name = 'TODO: ProjectName';
					$this->description = 'TODO: ProjectDesc';
					$this->user = NULL;
					$this->records = array();
					// TODO: fill record array
					for($i = rand(0,20); $i < 20; $i++)
						$this->records[] = rand(1,100);
				} else {

					if(!$current_user->access('view_records',$project)) return ERROR_COLLECTION_ACCESS_DENIED;

					$this->collection = 0;
					$this->project = $project;
					$this->name = '';
					$this->description = '';
					$this->user = $current_user->getUser();
					$this->records = array();
				}
			}
		} else {
			// init existing collection from DB
			// TODO: get data from DB

			$result = mysql_query("SELECT * FROM `collections` WHERE `collection` = $collection;");
			if(!$result = mysql_fetch_object($result)) return ERROR_COLLECTION_ACCESS_DENIED;

			$this->collection = $collection;
			$this->project = $result->project;
			$this->name = $result->name;
			$this->description = $result->description;
			$this->user = $result->user;

			$this->records = json_decode( $result->records );
		}
	}

	public function getCollectionMetaData() {
		$r = array();
		$r['name'] = $this->name;
		$r['description'] = $this->description;
		$r['collection'] = $this->collection;
		$r['num_records'] = count($this->records);

		return $r;
	}

	public function setName($_name) {
		if($_name != '') { // TODO: validate $_name
			$this->name = $_name;
		} else {
			return ERROR_INVALID_COLLECTION_NAME;
		}
	}

	public function setDescription($_description) {
		if(1) { // TODO: validate $_description
			$this->description = $_description;
			return true;
		} else {
			return ERROR_INVALID_COLLECTION_DESCRIPTION;
		}
	}

	public function getRecords() {
		return count($this->records) ? $this->records : FALSE;
	}

	public function linkRecord($record) {
		if(1/*TODO: check users permission to access this record*/) {
			if(array_search($record, $this->records) === FALSE) {
				$this->records[] = $record;
				return TRUE;
			} else {
				return ERROR_COLLECTION_LINK_DUPLICATE_RECORD;
			}
		} else return ERROR_COLLECTION_LINK_RECORD_ACCESS_DENIED;
	}

	public function unlinkRecord($record) {
		if(($key = array_search($record, $this->records)) !== FALSE) {
			unset($this->records[$key]);
			$this->records = array_values($this->records);
			return TRUE;
		} else return ERROR_COLLECTION_UNLINK_UNEXISTING_RECORD;
	}

	public function saveCollection() {
		if($this->name == '') return ERROR_INVALID_COLLECTION_NAME;

		if($this->collection == 0) { // new collection, sql INSERT

			$n = mysql_real_escape_string($this->name);
			$d = mysql_real_escape_string($this->description);
			$r = mysql_real_escape_string(json_encode($this->records));

			mysql_query("INSERT INTO `collections` (`name`, `description`, `user`, `project`, `records`) VALUES ('$n', '$d', '{$this->user}', '{$this->project}', '$r');");

			$this->collection = mysql_insert_id();

		} else { // existing collection, sql UPDATE
			if($this->name == '') return ERROR_INVALID_COLLECTION_NAME;

			$n = mysql_real_escape_string($this->name);
			$d = mysql_real_escape_string($this->description);
			$r = mysql_real_escape_string(json_encode($this->records));

			mysql_query("UPDATE `collections` SET `name` = '$n', `description` = '$d', `records` = '$r' WHERE `collection` = {$this->collection};");
		}
	}

	public function deleteCollection() {
		if($this->collection == 0) { // collection can't be deleted

		} else { // existing collection, sql UPDATE deleted flag
			mysql_query("DELETE FROM `collections` WHERE `collection` = {$this->collection}");
			$this->collection = 0;
		}
	}

}

?>