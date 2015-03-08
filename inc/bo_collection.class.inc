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

	function __construct($collection = 0, $project = 0) {
		if($collection == 0) {
			if($project  == 0) {
				return ERROR_COLLECTION_ACCESS_DENIED;
			} else {
				// init virtual collection with all records from the given project
				// TODO: check permission for user -> project
				$this->collection = 0;
				$this->project = $project;
				$this->name = 'TODO: ProjectName';
				$this->description = 'TODO: ProjectDesc';
				$this->user = NULL;
				$this->records = array();
				// TODO: fill record array
				for($i = rand(0,20); $i < 20; $i++)
					$this->records[] = rand(1,100);
			}
		} else {
			// init existing collection from DB
			// TODO: get data from DB

			$this->collection = $collection;
			$this->project = rand(0,100);
			$this->name = 'Collection #'.$collection;
			$this->description = 'DESC Collection #'.$collection;
			$this->user = rand(0,100);

			$this->records = array();
			for($i = rand(0,12); $i < 12; $i++)
				$this->records[] = rand(1,100);
		}
	}

	public function getCollectionMetaData() {
		$r = array();
		$r['name'] = $this->name;
		$r['description'] = $this->description;
		$r['collection'] = $this->collection;

		return $r;
	}

	public function setName($_name) {
		if(1) { // TODO: validate $_name
			$this->name = $_name;
			return true;
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
		if($this->collection == 0) { // new collection, sql INSERT

		} else { // existing collection, sql UPDATE

		}
	}

	public function deleteCollection() {
		if($this->collection == 0) { // collection can't be deleted

		} else { // existing collection, sql UPDATE deleted flag

		}
	}

}

?>