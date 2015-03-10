<?php

class vo {
	protected $html = '';
	protected $title;
	protected $path;
	protected $areaContent = array();
	protected $toasts = array('error' => array(), 'warning' => array(), 'info' => array());

	private $addedJs = array(); // array of all JS files which will be added
	private $addedCss = array(); // array of all CSS files which will be added
	private $addedJsSettings = array(); // array of all JS settings to pass
	private $activeMenuTrail;

	function __construct() {
		global $current_user;
		global $current_project;

		// set theme path
		$this->path = 'theme/'.THEME;

		// show login or logout form
		if($current_user->getUser() > 0) {
			$logout_form = file_get_contents($this->path.'/logout_form.tmpl');
			
			$search = array('%user-name%');
			$replace = array($current_user->getName());
			$logout_form = str_replace($search, $replace, $logout_form);

			$this->setAreaContent('login', $logout_form );
		} else {
			$this->addJs('libs/sha512.js');
			$this->addJs('login.js');
			$login_form = file_get_contents($this->path.'/login_form.tmpl');
			$this->setAreaContent('login', $login_form );
		}

		// build project menu
		$project_nav = file_get_contents($this->path.'/project_nav.tmpl');
		$link = file_get_contents($this->path.'/project_nav_entry.tmpl');

		#<a href="%href%" class="%active%">##%title%##</a><br>
		$search = array('%href%', '%active%', '%title%', '%sub-title%');

		$replace = array('manageProjects.php?project=-1', '%active-project-overview%', 'Projects', 'Overview over all projects');
		$projectOverview = str_replace($search, $replace, $link);
		$project_nav = str_replace('%links%', $projectOverview.'%links%', $project_nav);

		if(isset($current_project) && $current_project->getProject() > 0) {
			$replace = array('manageProjects.php', '%active-project-details%', 'Project Details', 'Details for the current project');
			$projectOverview = str_replace($search, $replace, $link);
			$project_nav = str_replace('%links%', $projectOverview.'%links%', $project_nav);
			
			$replace = array('manageRecords.php', '%active-project-records%', 'Records', 'View records');
			$projectOverview = str_replace($search, $replace, $link);
			$project_nav = str_replace('%links%', $projectOverview.'%links%', $project_nav);

			$replace = array('manageCollections.php', '%active-project-collections%', 'Collections', 'Collections of data records');
			$projectOverview = str_replace($search, $replace, $link);
			$project_nav = str_replace('%links%', $projectOverview.'%links%', $project_nav);

			if( $current_user->access('edit_record_structure', $current_project->getProject()) ) {
				$replace = array('manageRecordStructure.php', '%active-project-record-structure%', 'Record Structure', 'ToDo: text');
				$projectOverview = str_replace($search, $replace, $link);
				$project_nav = str_replace('%links%', $projectOverview.'%links%', $project_nav);
			}

			if( $current_user->access('manage_groups',$current_project->getProject()) ) {
				$replace = array('manageGroups.php', '%active-project-groups%', 'Groups', 'ToDo: text');
				$projectOverview = str_replace($search, $replace, $link);
				$project_nav = str_replace('%links%', $projectOverview.'%links%', $project_nav);
			}
		}

		$project_nav = str_replace('%links%', '', $project_nav);
		$this->setAreaContent('project_nav', $project_nav );

		// process global toasts
		if(isset($_SESSION['toasts'])) {
			foreach ($_SESSION['toasts'] as $class => $global_toast) {
				foreach ($global_toast as $msg) {
					$this->toast($msg, $class);
				}
			}
			unset($_SESSION['toasts']);
		}
	}

	private function translate() {
		if(file_exists ( 'locale/'.LOCALE.'.lang.inc' )) require_once 'locale/'.LOCALE.'.lang.inc';
		else $translation = array();

		$pattern = '~##(.*)##~sU';
		preg_match_all($pattern, $this->html, $matches);

		$search = array();
		$replace = array();
		foreach ($matches[0] as $key => $value) {
			$search[] = $value;
			if(isset($translation[ $matches[1][$key] ]))
				$replace[] = $translation[ $matches[1][$key] ];
			else
				$replace[] = $matches[1][$key];

		}

		$this->html = str_replace($search, $replace, $this->html);

	}

	protected function buildTable($structure) {
		$tb = '';

		$buttons = '';

		foreach ($structure['settings']['buttons'] as $button)
			$buttons .= '<a href="'.$button['href'].'" class="button" id="'.$button['id'].'">'.$button['title'].'</a>';
		if($structure['settings']['order'] == 'sort') {
			$buttons .= '<a href="javascript:void(0);" class="button" id="orderButton">Save order</a>';
			$tb .= '<form id="orderForm" method="post" action="?action=submitOrderForm"><input type="hidden" name="order" id="order"></form>';
		}

		$tb .= $buttons;

		if($structure['settings']['order'] == 'sort') {
			$tb .= '<div class="tipChangesNotSaved">New order of this table is not saved yet.</div>';	
		}

		$tb .= '<table class="data">';
		$tb .= '<thead><tr>';
		$tb .= '<th style="width: 24px;"><input type="checkbox" id="checkall"></th>';

		if($structure['settings']['order'] == 'sort')
			$tb .= '<th style="width: 24px;">&nbsp;</th>';

		foreach ($structure['settings']['header'] as $cell) {
			if(isset($cell['width']))
				$tb .= '<th width="'.$cell['width'].'">';
			else
				$tb .= '<th>';
			$tb .= $cell['title'];
			$tb .= '</th>';
		}
		$tb .= '</tr></thead>';

		$tb .= '<tbody>';
		$odd_even = 'odd';
		foreach ($structure['content'] as $id => $row) {
			$tb .= '<tr class="'.$odd_even.'">';
			if($odd_even == 'odd') $odd_even = 'even';
			else $odd_even = 'odd';
			$tb .= '<td><input type="checkbox" id="'.$id.'"></td>';
			if($structure['settings']['order'] == 'sort')
				$tb .= '<td><div class="sortHandle">&nbsp;</div></td>';
			foreach ($row as $cell) {
				$tb .= '<td>';
				$tb .= $cell;
				$tb .= '</td>';
			}
			$tb .= '</tr>';
		}
		$tb .= '</tbody>';

		$tb .= '</table>';
		if($structure['settings']['order'] == 'sort')
			$tb .= '<div class="tipChangesNotSaved">New order of this table is not saved yet.</div>';
		
		$tb .= $buttons;

		return $tb;
	}

	public function _print() {
		$this->html = file_get_contents($this->path.'/scaffold.tmpl');

		// replace title
		$this->html = str_replace('%title%', $this->title, $this->html);

		// replace toasts
		$toast_html = '';
		foreach ($this->toasts as $type => $toasts) {
			if(count($toasts)) {
				$toast_html .= file_get_contents($this->path.'/toast.tmpl');
				$toast_html = str_replace('%toast-type%', $type, $toast_html);
				$toast_html = str_replace('%toast-message%', '<ul><li>'.implode('</li><li>', $toasts).'</li></ul>', $toast_html);
			}
		}
		$this->html = str_replace('%toast%', $toast_html, $this->html);

		// add CSS
		foreach ($this->addedCss as $css)
			$this->html = str_replace('%css-add%','<link rel="stylesheet" href="'.ROOT.'theme/'.THEME.'/styles/'.$css.'">%css-add%',$this->html);
		$this->html = str_replace('%css-add%','',$this->html);

		// add JS settings
		$this->html = str_replace('%js-add%','<script type="text/javascript">var settings = ' . json_encode($this->addedJsSettings) . '</script>%js-add%',$this->html);

		// add JS
		foreach ($this->addedJs as $js)
			$this->html = str_replace('%js-add%','<script src="'.ROOT.'theme/'.THEME.'/js/'.$js.'"></script>%js-add%',$this->html);
		$this->html = str_replace('%js-add%','',$this->html);

		global $current_project;
		if(isset($current_project))
			$this->setAreaContent('current-project',$current_project->getName());
		else
			$this->setAreaContent('current-project','-');
		// replace areas
		$search = array();
		$replace = array();
		foreach ($this->areaContent as $area => $content) {
			$search[] = '%area-'.$area.'%';
			$replace[] = $content;
		}
		$this->html = str_replace($search, $replace, $this->html);

		// delete all unused areas
		$this->html = preg_replace('~%area-(.+?)%~', '', $this->html);

		// replace path
		$this->html = str_replace('%path%', $this->path, $this->html);
		
		// set active menu trail
		if(isset($this->activeMenuTrail)) $this->html = str_replace("%active-{$this->activeMenuTrail}%", 'active', $this->html);
		$this->html = preg_replace('~%active-(.+?)%~', '', $this->html);

		// delete all html comments
		$count = 1;
		while ($count > 0) $this->html = preg_replace('~<!--((?!<!--).)*?-->~s', '', $this->html, -1, $count);

		$this->translate();
		print $this->html;
	}

    public function addJs($js, $settings = null) {
    	if($js == 'settings') {
    		$this->addedJsSettings = array_merge($this->addedJsSettings, $settings);
    	} else $this->addedJs[] = $js;
	}

	public function addCss($css) {
    	$this->addedCss[] = $css;
	}

	public function setAreaContent($area, $content) {
		$this->areaContent[$area] = $content;
	}

	public function toast($message, $type = 'info') {
		$this->toasts[$type][] = $message;
	}

	public function setActiveMenuTrail($_trail) {
		$this->activeMenuTrail = $_trail;
	}

	public function _goto($target) {

		$_SESSION['toasts'] = $this->toasts;
		header("LOCATION: $target");
		exit();
	}
}

?>