<?php
require_once 'vo.abstract.class.inc';

class vo_manageCollections extends vo {
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageRecords/content.tmpl') );
	}

	public function showManageRecordStructureForm($data) {

		global $current_user;
		global $current_project;

		$this->addJS('manage_records.js');

		$content = '';


		$header = array();
		$rs = array();
		foreach ($data['rs'] as $col) {
			$header[] = array('title' => $col->title);
			$rs[$col->col_name] = $col;
		}

		$rows = array();
		foreach ($data['records'] as $record) {
			$row = array();

			foreach ($record['project_data'] as $key => $value) {
				switch ($rs[$key]->type) {
					case 'int':
					case 'text':
						if(isset($value)) $row[] = $value;
						else $row[] = '-';
						break;
					case 'timestamp':
						if(isset($value)) $row[] = $value != '' ? date("F j, Y", strtotime($value)) : '';
						else $row[] = '-';
						break;
					case 'double':
						if(isset($value)) $row[] = number_format($value, $rs[$key]->decimal_places, '.', ' ');
						else $row[] = '-';
						break;
					case 'boolean':
						$row[] = $value ? '<span class="ui-icon ui-icon-check" style="display: inline-block;"></span>' : '<span class="ui-icon ui-icon-closethick" style="display: inline-block;"></span>';
						break;
					
					default:
						# code...
						break;
				}
			}

			$rows[$record['record']] = $row;
		}

		$buttons = array();
		if($data['collection'] == -1) {

			$addEditForm = file_get_contents($this->path.'/manageRecords/add_edit_record_form.tmpl');

			foreach ($rs as $col) {

				switch ($col->type) {
					case 'int':
					case 'double':
					case 'text':
						$input = file_get_contents($this->path.'/manageRecords/add_edit_record_form_int_text_double.tmpl');
						break;
					case 'timestamp':
						$input = file_get_contents($this->path.'/manageRecords/add_edit_record_form_timestamp.tmpl');
						break;
					case 'boolean':
						$input = file_get_contents($this->path.'/manageRecords/add_edit_record_form_boolean.tmpl');
						break;
					
					default:
						# code...
						break;
				}

				$input = str_replace('%title%', $col->title, $input);
				$input = str_replace('%id%', $col->col_name, $input);

				$addEditForm = str_replace('%fields%', $input.'%fields%', $addEditForm);
			}

			$addEditForm = str_replace('%fields%', '', $addEditForm);

			$content .= $addEditForm;

			$content .= file_get_contents($this->path.'/manageRecords/delete_record_form.tmpl');
			
			$addToCollectionForm = file_get_contents($this->path.'/manageRecords/add_record_to_collection_form.tmpl');
			### collections
			foreach ($data['collections'] as $id => $name) {
				$addToCollectionForm = str_replace('%options%', '<option value="'.$id.'">'.$name.'</option>%options%', $addToCollectionForm);
			}
			$addToCollectionForm = str_replace('%options%', '', $addToCollectionForm);
			$content .= $addToCollectionForm;

			$this->addJS('settings', array('record_structure' => $rs));

			$buttons[] = array(
						'href' => 'javascript:void(0);',
						'title' => 'Add record',
						'id' => 'addRecordButton');
			$buttons[] = array(
						'href' => 'javascript:void(0);',
						'title' => 'Edit record',
						'id' => 'editRecordButton');
			$buttons[] = array(
						'href' => 'javascript:void(0);',
						'title' => 'Delete records',
						'id' => 'deleteRecordButton');
			$buttons[] = array(
						'href' => 'javascript:void(0);',
						'title' => 'Add to Collection',
						'id' => 'addRecordToCollectionButton');
		}

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'header' => $header,
				'buttons' => $buttons,
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		// $content .= '<pre>';
		// $content .= var_export($data,1);
		// $content .= '</pre>';

		$this->setAreaContent('records-form', $content);
	}
}
?>