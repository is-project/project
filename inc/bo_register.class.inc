<?php

class bo_register {
	/* properties */
	private $register;
	private $project;
	private $name;
	private $description;
	private $structure;

	/* methods */
	function __construct($register = 0){
		print 'Ich wurde angelegt und bin Register #'.$register;

		if($register == 0){
		// init empty register
		$this->register = 0;
		$this->project = null;
		} else {
			// init existing Register from DB

			//sql_query('SELECt FROM REGISTER WHERE id = register')	

			$this->register = $register;
			$this->project = rand(0,100);
			$this->name = 'Register #'.$register;
			$this->description = 'DESC Register #'.$register;
			//$this->structure = 'STRC Register #'.$register;

			$this->structure = array();
				for($i = rand(0,7); $i < 7; $i++)
					$this->structure[] = rand(1,100);
		}
	}

	//for testing
	public function getRegisterMetaData() {
		$r = array();
		$r['name'] = $this->name;
		$r['description'] = $this->description;
		$r['register'] = $this->register;

		return $r;
	}

	public function setName($_name){
		$this->name = $_name;

		return TRUE;
	}
	
	public function setDescription($_description){
		$this->description = $_description;

		return TRUE;
	}

	public function setStructure($_structure){
		$this->structure = $_structure;

		return TRUE;
	}

	public function saveRegister() {
		//sql_query('UPDATE register WHERE register = $this->register')

		return TRUE;
	}

	public function deletRegister(){
		#sql_query('DELETE FROM register WHERE register = this->register')

		return TRUE;
	}
}
?>