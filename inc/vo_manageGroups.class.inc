<?php
require_once 'vo.abstract.class.php';

class vo_manageGroups extends vo {

	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageGroups/content.tmpl') );
	}

	

	public function showManageGroupsForm($data) {

		$this->addJS('manage_groups.js');
		$this->addCSS('manage_groups.css');

		$this->addJS('settings', array('data' => $data));


		$content = '<div id="example"></div>';

		#<script src="http://handsontable.com/dist/jquery.handsontable.full.js"></script>
		#<link rel="stylesheet" media="screen" href="http://handsontable.com/dist/jquery.handsontable.full.css">
		#<link rel="stylesheet" media="screen" href="http://handsontable.com/demo/css/samples.css?20140401">

		$this->setAreaContent('groups-form', $content);
	}

	public function showGroupsOverview($groups) {
		$this->addJS('manage_groups.js');
		$this->addCSS('manage_groups.css');


		$content = '';

		$content .= file_get_contents($this->path.'/manageGroups/add_edit_groups_form.tmpl');
		$content .= file_get_contents($this->path.'/manageGroups/delete_groups_form.tmpl');

		$rows = array();
		$row = array();
		foreach ($groups as $row) {
			$tmp = new bo_group($row->group);

			$meta = $tmp->getGroupMetaData();
			$nrMember = 0;
			$nrPermission = 0;
			foreach ($meta['member'] as $m) {
				$nrMember = $nrMember + 1;
			}
			foreach ($meta['permission'] as $m) {
				$nrPermission = $nrPermission + 1;
			}

			$settings = '
				<a href="#">
					<span class="ui-icon ui-icon-pencil" style="display: inline-block;"></span>
					<span style="display: inline-block; overflow: hidden;">Edit Members</span>
				</a>
				 | 
				<a href="#">
					<span class="ui-icon ui-icon-pencil" style="display: inline-block;"></span>
					<span style="display: inline-block; overflow: hidden;">Edit Permissions</span>
				</a>';

			$rows[] = array($meta['name'],$nrMember,$nrPermission, $settings);
		}

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'addEntry' => 'add_edit_groups_form_dialog',
				'editEntry' => 'add_edit_groups_form_dialog',
				'delEntry' => 'delete_groups_form_dialog',
				'header' => array(
								array(
									'title' => 'Group',
									'width' => '100px',
									'wordWrap' => TRUE),
								array(
									'title' => 'Members'),
								array(
									'title' => 'Permissions',
									'width' => '50px'),
								array(
									'title' => 'Settings')
							),
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		$this->setAreaContent('groups-form', $content);
	}



	}
?>