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
	private $userFields;

	/* methods */

	function __construct($record = 0) 
	{
		print 'Ich wurde angelegt, bin Record #'.$record;

		if($record == 0) 
		{
			// init empty record
			$this->record = $record;
			$this->deleted = null;
			$this->deletedBy = null;
			$this->created = null;
			$this->createdBy = null;
			$this->params = null;
		} 
		else 
		{
			$result = mysql_query('SELECT * FROM data_project_1 WHERE record = '.$record);
			if (!$result) {
				echo mysql_error();
			}
			
			$fields = mysql_num_fields($result);
			$row = mysql_fetch_array($result, MYSQL_BOTH);	
			$this->userFields = $fields-5;	
			
			$this->record = $record;
			$this->deleted = $row['deleted'];
			$this->deletedBy = $row['deleted_by'];
			$this->created = $row['created'];
			$this->createdBy = $row['created_by'];
			$this->params = array();
			for($i = 0; $i < ($this->userFields); $i++)
			{
				$keys = array_keys($row);
				$this->params[] = $row[$i+1];
			}
		}
	}

	public function getRecord() 
	{			
		$return = array( 'record' => $this->record, 'deleted' => $this->deleted, 'deleted_by' => $this->deletedBy, 'created' => $this->created, 'created_by' => $this->createdBy, 'project' => 1, 'project_data' => $this->params);
		return $return;
	}
	
	public function setParams($params)
	{
		if((count($params) !== $this->userFields)&&($this->record !== 0))
			return PARAM_LIST_INVALID;
		
		$this->params = $params;
		
		if($this->record == 0)
		{			
			$this->created = time();
			$this->createdBy = 1;
		}

		return true;
	}
	
	public function saveRecord()
	{
		if($this->record == 0) //Update
		{

		}
		else //Insert
		{

		}
	
		return true;
	}
	
	public function deleteRecord()
	{
		if($this->record == 0)
			return CANT_DELETE_RECORD;
		$time=date("Y-m-d H:i:s",time()); 
		$result = mysql_query('UPDATE data_project_1 SET deleted = "'.$time.'", deleted_by = 1 WHERE record = '.$this->record);
		if (!$result) {
			return mysql_error();
		}
		return true;
	}
}

?>