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
		// $this->html = file_get_contents($this->path.'/scaffold.tmpl');
		$this->title = 'Manage Groups';
		$this->addJS('manage_groups.js');
		$this->addCSS('manage_groups.css');

		$groupListHtml = $this->buildGroupsView($groups);

		$groupForm = file_get_contents($this->path.'/manageGroups/add_edit_group_form.tmpl');

		$content = '
			<table>
				<tr>
					<td>'.$groupListHtml.'</td>
					<td>'.$groupForm.'</td>
				</tr>
			</table>';

		$this->setAreaContent('listOfGroups', $content);
	}

	}
?>