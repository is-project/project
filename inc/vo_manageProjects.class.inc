<?php
require_once 'vo.abstract.class.php';

class vo_manageProjects extends vo {
	
	
	function __construct() {
		parent::__construct();
	}

	private function buildProjectsTree($projects) {
		$html = '<ul>';
		foreach ($projects as $key => $value) {
			$html .= '<li><span title="'.$value['description'].'" project="'.$value['project'].'" parent-project="'.$value['parent_project'].'" edit-access="'.$value['edit_access'].'">'.$value['name'].'</span></li>';
			if(count($value['children']))
				$html .= $this->buildProjectsTree($value['children']);
		}
		$html .= '</ul>';

		return $html;
	}

	public function showProjectOverview($projects) {
		$this->html = file_get_contents($this->path.'scaffold.tmpl');
		$this->title = 'Manage Projects';

		$projectListHtml = $this->buildProjectsTree($projects);

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