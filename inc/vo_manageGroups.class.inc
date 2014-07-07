<?php
require_once 'vo.abstract.class.php';

class vo_manageGroups extends vo {

	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageGroups/content.tmpl') );
	}

	
?>