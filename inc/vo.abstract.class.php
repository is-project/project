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

	function __construct() {
		$this->path = 'theme/'.THEME;

		global $current_user;
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

		// global $current_project;
		// if(isset($current_project)) {
			$project_nav = file_get_contents($this->path.'/project_nav.tmpl');
			$this->setAreaContent('project_nav', $project_nav );
		// }

		global $global_toasts;
		if(isset($global_toasts))
			foreach ($global_toasts as $class => $global_toast) {
				foreach ($global_toast as $msg) {
					$this->toast($msg, $class);
				}
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

	public function _print() {
		$this->html = file_get_contents($this->path.'/scaffold.tmpl');

		// replace title
		$this->html = str_replace('%title%', $this->title, $this->html);

		// replace path
		$this->html = str_replace('%path%', $this->path, $this->html);

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

		// delete all html comments
		$this->html = preg_replace('~<!--(.*?)-->~s', '', $this->html);

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
}

?>