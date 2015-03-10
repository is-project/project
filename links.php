<?php

require_once 'inc/init.inc';
require_once 'inc/vo.abstract.class.php';
require_once 'inc/bo_user.class.inc';
// require_once 'inc/bo_project.class.inc';

global $current_user;
// global $current_project;

class vo_textPage extends vo {
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/textPage/links.tmpl') );
	}
}

$layout = new vo_textPage();

$layout->setActiveMenuTrail('links-trail');

$layout->_print();

?>