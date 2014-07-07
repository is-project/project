<?php
require_once 'vo.abstract.class.php';

class vo_manageRecordStructure extends vo {
	
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageRecordStructure/content.tmpl') );
	}
	
	public function showManageRecordStructureForm($data) {

		$this->addJS('manage_record_structure.js');
		$this->addCSS('manage_record_structure.css');
		$this->addJS('libs/jquery.handsontable.full.js');
		$this->addCSS('jquery.handsontable.full.css');

		$this->addJS('settings', array('data' => $data));


		$content = '<div id="example"></div>';

		#<script src="http://handsontable.com/dist/jquery.handsontable.full.js"></script>
		#<link rel="stylesheet" media="screen" href="http://handsontable.com/dist/jquery.handsontable.full.css">
		#<link rel="stylesheet" media="screen" href="http://handsontable.com/demo/css/samples.css?20140401">

		$this->setAreaContent('record-structure-form', $content);
	}

}

?>