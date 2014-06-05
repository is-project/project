<?php
require_once 'vo.abstract.class.php';

class vo_manageProjects extends vo {
	
	function __construct() {
		parent::__construct();
	}

	public function showProjectOverview($projects) {
		$this->html = file_get_contents($this->path.'scaffold.tmpl');
		$this->title = 'Manage Projects';

		$projectListHtml = '<ul>';
		foreach ($projects as $key => $value) {
			$projectListHtml .= '<li><span title="'.$value['description'].'">'.$value['name'].'</span></li>';
		}
		$projectListHtml .= '</ul>';

		$projectForm = file_get_contents($this->path.'manageProjects/addEditProjectForm.tmpl');

		$content = '
			<table>
				<tr>
					<td>'.$projectListHtml.'</td>
					<td>'.$projectForm.'</td>
				</tr>
			</table>';

		$this->setAreaContent('content', $content);
		$this->_print();
	}
}

?>