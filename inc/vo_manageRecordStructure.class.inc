<?php
require_once 'vo.abstract.class.inc';

class vo_manageRecordStructure extends vo {
	
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageRecordStructure/content.tmpl') );
	}
	
	public function showManageRecordStructureForm($data) {

		$this->addJS('manage_record_structure.js');
		#$this->addCSS('manage_record_structure.css');

		$content = '';

		// add / edit form

		$content .= file_get_contents($this->path.'/manageRecordStructure/add_edit_record_structure_form.tmpl');
		$content .= file_get_contents($this->path.'/manageRecordStructure/delete_record_structure_form.tmpl');

		$rows = array();
		foreach ($data as $row) {
			$rows[$row[0]] = array($row[1],$row[2],$row[3],$row[4]);
		}

		$tb_content = array(
			'settings' => array(
				'order' => 'sort', // none, header, sort
				'header' => array(
								array(
									'title' => 'Title',
									'wordWrap' => TRUE),
								array(
									'title' => 'Type'),
								array(
									'title' => 'Length'),
								array(
									'title' => 'Decimal Places',
									'wordWrap' => TRUE)
							),
				'buttons' => array(
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Add record structure coloumn',
									'id' => 'addRecordStructureButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Edit record structure coloumn',
									'id' => 'editRecordStructureButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Delete record structure coloumn',
									'id' => 'deleteRecordStructureButton'),
							),
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		$this->setAreaContent('record-structure-form', $content);
	}

}

?>