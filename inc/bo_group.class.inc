<?php

class bo_group {
	/* properties */

	private $group;
	private $project;
	private $name;
	private $description;
	private $members;

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

			$this->members = array();
			for($i = rand(0,7); $i < 7; $i++)
				$this->members[] = rand(1,100);
		}
	}

	public function getGroupMetaData() {
		$r = array();
		$r['name'] = $this->name;
		$r['description'] = $this->description;
		$r['group'] = $this->group;

		return $r;
	}

}

?>