<?php
require_once 'vo.abstract.class.php';

class vo_manageProjects extends vo {
	
	function __construct() {
		parent::__construct();
		global $current_user;
		$this->setAreaContent('content', file_get_contents($this->path.'/manageProjects/content.tmpl') );
	}

	private function buildProjectsTree($projects, $level = 0) {
		
		$html = $level ? '' : '<table class="projects">';
		$entry = file_get_contents($this->path.'/manageProjects/project_entry.tmpl');
		$search = array('%project%', '%project-name%', '%project-description%', '%parent-project%', '%edit-access%', '%create-access%', '%delete-access%');

		preg_match_all('~%level-indent-x([0-9]+)%~', $entry, $matches);

		$search = array_merge($search, $matches[0]);		
		foreach ($matches[1] as $key => $value) {
			$matches[1][$key] = $level * $value;
		}

		foreach ($projects as $key => $value) {
			$replace = array($value['project'], $value['name'], $value['description'], $value['parent_project'], $value['edit_access'], $value['create_access'], $value['delete_access']);
			$replace = array_merge($replace, $matches[1]);
			$html .= str_replace($search, $replace, $entry);
			if(count($value['children']))
				$html .= $this->buildProjectsTree($value['children'], $level+1);
		}
		$html .= $level ? '' : '</table>';

		return $html;
	}

	public function showProjectsOverview($projects) {
		// $this->html = file_get_contents($this->path.'/scaffold.tmpl');
		$this->title = 'Manage Projects';
		$this->addJS('manage_projects.js');
		$this->addCSS('manage_projects.css');

		$projectListHtml = $this->buildProjectsTree($projects);

		$projectForm = file_get_contents($this->path.'/manageProjects/add_edit_project_form.tmpl');

		$content = '
			<table>
				<tr>
					<td>'.$projectListHtml.'</td>
					<td>'.$projectForm.'</td>
				</tr>
			</table>';

		$this->setAreaContent('listOfProjects', $content);
	}

	public function showProjectDeleteForm($project, $children) {

		$content = file_get_contents($this->path.'/manageProjects/delete_project_form.tmpl');

		$tmp = array();
		foreach ($children as $child)
			$tmp[] = $child['name'];

		$search = array('%project%', '%project-name%', '%project-description%', '%project-children%');
		$replace = array($project['project'], $project['name'], $project['description'], implode(', ', $tmp));

		$content = str_replace($search, $replace, $content);

		$this->setAreaContent('deleteProject', $content);
	}

	public function showProjectOverview() {
		// $this->html = file_get_contents($this->path.'/scaffold.tmpl');
		$this->title = 'Manage Projects';
		$this->addJS('manage_projects.js');
		$this->addCSS('manage_projects.css');

		$projectForm = file_get_contents($this->path.'/manageProjects/add_edit_project_form.tmpl');

		$this->setAreaContent('content', $projectForm);
	}
}

?>