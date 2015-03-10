$(document).ready(function () {

	$( ".date" ).datepicker({
		'dateFormat': "MM d, y"
	});

	$( "#add_edit_record_form_dialog" ).dialog({
		autoOpen: false,
		height: 450,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#add_edit_record_form" ).submit();
		},
		Cancel: function() {
			$( "#add_edit_record_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#add_edit_record_form input#record" ).val('');
			$( "#add_edit_record_form input[type=text]" ).val('');
			$( "#add_edit_record_form input[type=checkbox]" ).prop('checked','');

			$( "#add_edit_record_form_dialog p.error" ).html( '' );
		}
	});

	$( "a#addRecordButton" ).click(function(){
		$( "#add_edit_record_form_dialog" ).dialog('open');
	});


});