<?php
require_once 'vo.abstract.class.php';

class vo_manageCollections extends vo {
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageCollections/content.tmpl') );
	}

	public function showManageRecordStructureForm($data) {

		$this->addJS('manage_collections.js');

		$content = '';

		$content .= file_get_contents($this->path.'/manageCollections/add_edit_collection_form.tmpl');
		$content .= file_get_contents($this->path.'/manageCollections/delete_collection_form.tmpl');

		$rows = array();
		foreach ($data as $collection) {
			$rows[$collection['collection']] = array($collection['name'], $collection['description'], $collection['num_records']);
		}

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'header' => array(
								array(
									'title' => 'Name',
									'wordWrap' => TRUE),
								array(
									'title' => 'Description'),
								array(
									'title' => 'Number of records',)
							),
				'buttons' => array(
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Add collection',
									'id' => 'addCollectionButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Edit collection',
									'id' => 'editCollectionButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => '##Delete collection##',
									'id' => 'deleteCollectionButton'),
							),
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		$this->setAreaContent('collections-form', $content);
	}
}