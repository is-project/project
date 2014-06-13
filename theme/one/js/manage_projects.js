$(document).ready(function() {

	if(settings['form-errors'] != undefined) {
		for(i in settings['form-errors']) {
			$('form#add-edit-project-form #' + i).addClass('error');
		}
	}

	if(settings['form-status'] != undefined) {
		if(settings['form-status'] == 'edit') {
			$('form#add-edit-project-form input#parent-project').attr('readonly', true);
			$('form#add-edit-project-form input#parent-project-name').attr('readonly', true);
			$('form#add-edit-project-form input#project-name').attr('readonly', false);
			$('form#add-edit-project-form textarea#project-description').attr('readonly', false);
			$('form#add-edit-project-form input[type=submit]').attr('disabled', false);
		} else if(settings['form-status'] == 'add') {
			$('form#add-edit-project-form input#parent-project').attr('readonly', true);
			$('form#add-edit-project-form input#parent-project-name').attr('readonly', true);
			$('form#add-edit-project-form input#project-name').attr('readonly', false);
			$('form#add-edit-project-form textarea#project-description').attr('readonly', false);
			$('form#add-edit-project-form input[type=submit]').attr('disabled', false);
		}
	}

	if(settings['form-values'] != undefined) {
		for(i in settings['form-values']) {
			$('form#add-edit-project-form #' + i).val(settings['form-values'][i]);
		}
	}

	$('table.projects td.project-settings a.details.access-1').click(function() {
		$('form#add-edit-project-form input#parent-project').attr('readonly', true);
		$('form#add-edit-project-form input#parent-project-name').attr('readonly', true);
		$('form#add-edit-project-form input#project-name').attr('readonly', true);
		$('form#add-edit-project-form textarea#project-description').attr('readonly', true);
		$('form#add-edit-project-form input[type=submit]').attr('disabled', true);
		fillForm($(this).closest('tr'));
	});

	$('table.projects td.project-settings a.edit.access-1').click(function() {
		$('form#add-edit-project-form input#parent-project').attr('readonly', true);
		$('form#add-edit-project-form input#parent-project-name').attr('readonly', true);
		$('form#add-edit-project-form input#project-name').attr('readonly', false);
		$('form#add-edit-project-form textarea#project-description').attr('readonly', false);
		$('form#add-edit-project-form input[type=submit]').attr('disabled', false);
		fillForm($(this).closest('tr'));
	});

	$('table.projects td.project-settings a.subproject.access-1').click(function() {
		$('form#add-edit-project-form input#parent-project').attr('readonly', true);
		$('form#add-edit-project-form input#parent-project-name').attr('readonly', true);
		$('form#add-edit-project-form input#project-name').attr('readonly', false);
		$('form#add-edit-project-form textarea#project-description').attr('readonly', false);
		$('form#add-edit-project-form input[type=submit]').attr('disabled', false);

		var tr = $(this).closest('tr');
		var project = tr.attr('project');
		var project_name = tr.find('td.project-name span').text();
		$('form#add-edit-project-form input#project').val( ' < NEW > ' );
		$('form#add-edit-project-form input#parent-project').val( project );
		$('form#add-edit-project-form input#parent-project-name').val( project_name );
		$('form#add-edit-project-form input#project-name').val( '' );
		$('form#add-edit-project-form textarea#project-description').val( '' );
	});

});

function fillForm(tr) {
	var project = tr.attr('project');
	var parent_project = tr.attr('parent-project');
	var parent_project_name = (parent_project != 0) ? $('table.projects tr.project[project='+parent_project+'] td.project-name span').text() : ' < ROOT > ';
	var project_name = tr.find('td.project-name span').text();
	var project_description = tr.find('td.project-name span').attr('title');

	$('form#add-edit-project-form input#project').val( project );
	$('form#add-edit-project-form input#parent-project').val( parent_project );
	$('form#add-edit-project-form input#parent-project-name').val( parent_project_name );
	$('form#add-edit-project-form input#project-name').val( project_name );
	$('form#add-edit-project-form textarea#project-description').val( project_description );
}