$(document).ready(function() {

	$('table.projects td.project-settings a.details.access-1').click(function() {
		$('form#add-edit-project-form input#parent-project').attr('disabled', true);
		$('form#add-edit-project-form input#parent-project-name').attr('disabled', true);
		$('form#add-edit-project-form input#project-name').attr('disabled', true);
		$('form#add-edit-project-form textarea#project-description').attr('disabled', true);
		$('form#add-edit-project-form input[type=submit]').attr('disabled', true);
		fillForm($(this).closest('tr'));	
	});

	$('table.projects td.project-settings a.edit.access-1').click(function() {
		$('form#add-edit-project-form input#parent-project').attr('disabled', true);
		$('form#add-edit-project-form input#parent-project-name').attr('disabled', true);
		$('form#add-edit-project-form input#project-name').attr('disabled', false);
		$('form#add-edit-project-form textarea#project-description').attr('disabled', false);
		$('form#add-edit-project-form input[type=submit]').attr('disabled', false);
		fillForm($(this).closest('tr'));
	});

	$('table.projects td.project-settings a.subproject.access-1').click(function() {
		$('form#add-edit-project-form input#parent-project').attr('disabled', true);
		$('form#add-edit-project-form input#parent-project-name').attr('disabled', true);
		$('form#add-edit-project-form input#project-name').attr('disabled', false);
		$('form#add-edit-project-form textarea#project-description').attr('disabled', false);
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