<?php

class bo_record {
	/** properties */
	
	private $record;
	private $deleted;
	private $deletedBy;
	private $created;
	private $createdBy;
	private $project;
	private $params;

	/* methods */

	function __construct($project = 0, $record = 0, $formData = array()) 
	{
		
		$rs = mysql_query("SELECT `record_structure` FROM `projects` WHERE `project` = $project;");
		if(!$rs = mysql_fetch_object($rs)) return -901;
		$rs = json_decode($rs->record_structure);

		$this->project = $project;

		if($record == 0) 
		{
			// init empty record
			$this->record = $record;
			$this->deleted = null;
			$this->deletedBy = null;
			$this->created = null;
			$this->createdBy = null;
			
			$this->params = array();
			$this->setParams($formData);

		} else {

			$result = mysql_query('SELECT * FROM data_project_'.$project.' WHERE entry = '.$record.' AND `deleted` IS NULL');
			if (!$result) {
				echo mysql_error();
			}
			
			$fields = mysql_num_fields($result);
			$row = mysql_fetch_object($result);

			if(!$row) return -900;
			
			$this->record = $record;
			$this->deleted = $row->deleted;
			$this->deletedBy = $row->deleted_by;
			$this->created = $row->created;
			$this->createdBy = $row->created_by;
			$this->params = array();

			foreach ($rs as $param) {
				$this->params[$param->col_name] = $row->{$param->col_name};
			}

		}
	}

	public function getRecord() 
	{			
		$return = array( 'record' => $this->record, 'deleted' => $this->deleted, 'deleted_by' => $this->deletedBy, 'created' => $this->created, 'created_by' => $this->createdBy, 'project' => 1, 'project_data' => $this->params);
		return $return;
	}
	
	public function setParams($formData)
	{
		$rs = mysql_query("SELECT `record_structure` FROM `projects` WHERE `project` = {$this->project};");
		$rs = mysql_fetch_object($rs);
		$rs = json_decode($rs->record_structure);

		$this->params = array();
		foreach ($rs as $param) {
			if(isset($formData[$param->col_name])) {
				switch ($param->type) {
					case 'int':
						if(valid_int($formData[$param->col_name]))
							$this->params[$param->col_name] = $formData[$param->col_name];
						break;
					case 'double':
						if(is_numeric($formData[$param->col_name]))
							$this->params[$param->col_name] = floatval($formData[$param->col_name]);
						break;
					case 'text':
						if($formData[$param->col_name] != '')
							$this->params[$param->col_name] = $formData[$param->col_name];
						break;
					case 'boolean':
						$this->params[$param->col_name] = 1;
						break;
					case 'timestamp':
						if(strtotime($formData[$param->col_name]))
							$this->params[$param->col_name] = date("Y-m-d H:i:s",strtotime($formData[$param->col_name]));
						break;
				}
			}

			if(!isset($this->params[$param->col_name]))
				$this->params[$param->col_name] = null;
		}
	}
	
	public function saveRecord()
	{
		global $current_user;

		if($this->record == 0) //Insert
		{

			$keys = array('`created_by`');
			$vals = array($current_user->getUser());
			foreach ($this->params as $key => $value) {
				if(isset($value)) {
					$keys[] = '`'.$key.'`';
					$vals[] = "'".$value."'";
				}
			}

			$keys = implode(',', $keys);
			$vals = implode(',', $vals);

			mysql_query("INSERT INTO `data_project_{$this->project}` ($keys) VALUES ($vals)");

		}
		else //Update
		{
			$updates = array();
			foreach ($this->params as $key => $value) {
				if(isset($value)) {
					$updates[] = "`$key` = '$value'";
				} else {
					$updates[] = "`$key` = NULL";
				}
			}

			$updates = implode(',', $updates);

			mysql_query("UPDATE `data_project_{$this->project}` SET $updates WHERE `entry` = $this->record");
			#UPDATE `is-project`.`data_project_4` SET `deleted` = NULL, `param2` = 'Schnaps2', `param4` = NULL, `param10` = NULL WHERE `data_project_4`.`entry` = 14;
		}
	
		return true;
	}
	
	public function deleteRecord()
	{
		global $current_user;
		if($this->record == 0)
			return CANT_DELETE_RECORD;
		$time=date("Y-m-d H:i:s",time()); 
		$user = $current_user->getuser();
		$result = mysql_query('UPDATE `data_project_'.$this->project.'` SET `deleted` = "'.$time.'", `deleted_by` = '.$user.' WHERE entry = '.$this->record);
		if (!$result) {
			return mysql_error();
		}
		return true;
	}
}

?>