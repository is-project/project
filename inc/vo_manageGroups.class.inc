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
		$this->addJS('settings', array('data' => $data));


		$content = '<div id="example"></div>';

		#<script src="http://handsontable.com/dist/jquery.handsontable.full.js"></script>
		#<link rel="stylesheet" media="screen" href="http://handsontable.com/dist/jquery.handsontable.full.css">
		#<link rel="stylesheet" media="screen" href="http://handsontable.com/demo/css/samples.css?20140401">

		$this->setAreaContent('groups-form', $content);
	}

	public function showGroupsOverview($groups) {
		$this->addJS('manage_groups.js');

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
				<a href="?location=member&group='.$meta['group'].'">
					<span class="ui-icon ui-icon-pencil" style="display: inline-block;"></span>
					<span style="display: inline-block; overflow: hidden;">Edit Members</span>
				</a>
				 | 
				<a href="?location=permission&group='.$meta['group'].'">
					<span class="ui-icon ui-icon-pencil" style="display: inline-block;"></span>
					<span style="display: inline-block; overflow: hidden;">Edit Permissions</span>
				</a>';

			$rows[$meta['group']] = array($meta['name'],$nrMember,$nrPermission, $settings);
		}

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'header' => array(
								array(
									'title' => 'Group',
									'wordWrap' => TRUE),
								array(
									'title' => 'Members'),
								array(
									'title' => 'Permissions'),
								array(
									'title' => 'Settings',
									'width' => '330px'),
							),
				'buttons' => array(
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Add Group',
									'id' => 'addGroupButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Edit Group',
									'id' => 'editGroupButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Delete Group(s)',
									'id' => 'deleteGroupButton'),
							),
			),
			'content' => $rows
			);
		$content .= "<h1>Groups</h1>";
		$content .= $this->buildTable($tb_content);

		$this->setAreaContent('groups-form', $content);
	}

	public function showAccesDeniedForm()
	{
		$content = '<h1>Acces Denied</h1>';
		$this->setAreaContent('members-form', $content);
	}

	public function showEditMembersForm($group){
		$this->addJS('edit_members.js');
		$content = '';

		$tmp = new bo_group($group);

		$content .= '<h1>Membermanagement</h1><br>';
		$content .= '<h2>'.$tmp->getName().'</h2><br>';
		$content .= str_replace("%group-id%", $group, file_get_contents($this->path.'/manageGroups/add_members_form.tmpl')) ;
		$content .= str_replace("%group-id%", $group, file_get_contents($this->path.'/manageGroups/delete_members_form.tmpl'));

		$rows = array();
		
		

		$members = $tmp->getMembers();
		foreach ($members as $member) {
			$memberObj = new bo_user($member);
			$rows[$member] = array($memberObj->getName(),$memberObj->getEmail(),$memberObj->getDescription());
		}
		

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'header' => array(
								array(
									'title' => 'Name',
									'width' => '100px',
									'wordWrap' => TRUE),
								array(
									'title' => 'eMail'),
								array(
									'title' => 'Description',
									'width' => '100px',
									'wordWrap' => TRUE),
							),
				'buttons' => array(
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Add Member',
									'id' => 'addMembersButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Delete Member(s)',
									'id' => 'deleteMembersButton'),
							),
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		$this->setAreaContent('members-form', $content);
	}


	public function showEditPermissionsForm($group){
		$this->addJS('edit_permissions.js');
		$content = '';

		$groupObj = new bo_group($group);

		$content .= '<h1>Permissionmanagement</h1><br>';
		$content .= '<h2>'.$groupObj->getName().'</h2><br>';
		$permissionListGroup = array();
		$permissionListSelect = '';

		

		$permissionListGroup = $groupObj->getPermissions();

		$_permission = mysql_query("SELECT * FROM `permissions`");
		while($result = mysql_fetch_object($_permission)) {
			$inside = FALSE;
	    	foreach ($permissionListGroup as $p) {
				if ($result->permission == $p) $inside = TRUE;
			}
			if ($inside == FALSE) {
				$permissionListSelect .= '<option value="'.$result->permission.'">'.$result->name.'</option>';
			}
		}


		//kkkkk_kkkkk kkkkkkkkkkkk kkkkkkkkkk kkk^kkk


		$content .= str_replace("%permission-list%", $permissionListSelect ,str_replace("%group-id%", $group, file_get_contents($this->path.'/manageGroups/add_permissions_form.tmpl')));


		$content .= str_replace("%group-id%", $group, file_get_contents($this->path.'/manageGroups/delete_permissions_form.tmpl'));

		$rows = array();
		
		$tmp = new bo_group($group);


		foreach ($permissionListGroup as $_perm) {
			$_permission = mysql_fetch_object(mysql_query("SELECT * FROM `permissions` WHERE `permission` = $_perm"));

			$rows[$_permission->permission] = array($_permission->name,$_permission->description);
		}
		

		$tb_content = array(
			'settings' => array(
				'order' => 'header', // none, header, sort
				'header' => array(
								array(
									'title' => 'Name',
									'width' => '100px',
									'wordWrap' => TRUE),
								array(
									'title' => 'Description',
									'width' => '100px',
									'wordWrap' => TRUE),
							),
				'buttons' => array(
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Add permission',
									'id' => 'addPermissionsButton'),
								array(
									'href' => 'javascript:void(0);',
									'title' => 'Delete permission(s)',
									'id' => 'deletePermissionsButton'),
							),
			),
			'content' => $rows
			);

		$content .= $this->buildTable($tb_content);

		$this->setAreaContent('permissions-form', $content);
	}


	}
?>