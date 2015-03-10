<?php
require_once 'vo.abstract.class.php';

class vo_manageCollections extends vo {
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageRecords/content.tmpl') );
	}

	public function showManageRecordStructureForm($data) {

		#$this->addJS('manage_collections.js');

		$content = '';

		$header = array();
		foreach ($data['rs'] as $col) {
			$header[] = array('title' => $col->title);
		}

		$rows = array();
		foreach ($data['records'] as $record) {
			$row = array();

			foreach ($record['project_data'] as $value) {
				$row[] = $value;
			}

			$rows[$record['record']] = $row;
		}

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'header' => $header,
				'buttons' => array(),
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		$content .= '<pre>';
		$content .= var_export($data,1);
		$content .= '</pre>';

		$this->setAreaContent('records-form', $content);
	}
}
?>