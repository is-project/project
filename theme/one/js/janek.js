$(document).ready(function(){
	$('ul.projects li').on('click', function() {
		$('form#add-edit-project-form input#project').val( $(this).find('span:first').attr('project') );
		$('form#add-edit-project-form input#parent-project').val( $(this).find('span:first').attr('parent-project') );
		$('form#add-edit-project-form input#project-name').val( $(this).find('span:first').html() );
		$('form#add-edit-project-form textarea#project-description').val( $(this).find('span:first').attr('title') );
	});
});