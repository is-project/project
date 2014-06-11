<?php
require_once 'vo.abstract.class.php';

class vo_manageProjects extends vo {
	
	
	function __construct() {
		parent::__construct();
	}

	private function buildProjectsTree($projects) {
		$html = '<ul class="projects">';
		$search = array('%project%', '%project-name%', '%project-description%', '%parent-project%', '%edit-access%');
		foreach ($projects as $key => $value) {
			$replace = array($value['project'], $value['name'], $value['description'], $value['parent_project'], $value['edit_access']);
			$entry = file_get_contents($this->path.'manageProjects/project_entry.tmpl');
			$html .= str_replace($search, $replace, $entry);
			if(count($value['children']))
				$html .= $this->buildProjectsTree($value['children']);
		}
		$html .= '</ul>';

		return $html;
	}

	public function showProjectOverview($projects) {
		$this->html = file_get_contents($this->path.'scaffold.tmpl');
		$this->title = 'Manage Projects';
		$this->addJS('janek.js');

		$projectListHtml = $this->buildProjectsTree($projects);

		$projectForm = file_get_contents($this->path.'manageProjects/add_edit_project_form.tmpl');

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