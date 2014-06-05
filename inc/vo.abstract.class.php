<?php

class vo {
	protected $html = '';
	protected $title;
	protected $path;
	protected $areaContent = array();
	protected $toasts = array('error' => array(), 'warning' => array(), 'info' => array());

	function __construct() {
		$this->path = 'theme/'.THEME.'/';
	}

	protected function _print() {
		// replace title
		$this->html = str_replace('%title%', $this->title, $this->html);
		// replace path
		$this->html = str_replace('%path%', $this->path, $this->html);
		// replace toasts
		$toast_html = '';
		foreach ($this->toasts as $type => $toasts) {
			if(count($toasts)) {
				$toast_html .= file_get_contents($this->path.'toast.tmpl');
				$toast_html = str_replace('%toast-type%', $type, $toast_html);
				$toast_html = str_replace('%toast-message%', '<ul><li>'.implode('</li><li>', $toasts).'</li></ul>', $toast_html);
			}
		}
		$this->html = str_replace('%toast%', $toast_html, $this->html);


		// replace areas
		$search = array();
		$replace = array();
		foreach ($this->areaContent as $area => $content) {
			$search[] = '%area-'.$area.'%';
			$replace[] = $content;
		}
		$this->html = str_replace($search, $replace, $this->html);

		$this->translate();
		print $this->html;
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

	public function setAreaContent($area, $content) {
		$this->areaContent[$area] = $content;
	}

	public function toast($message, $type = 'info') {
		$this->toasts[$type][] = $message;
	}
}

?>