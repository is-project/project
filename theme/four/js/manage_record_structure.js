$(document).ready(function () {

	$( "#add_edit_record_structure_form_dialog" ).dialog({
		autoOpen: false,
		height: 350,
		width: 400,
		modal: true,
		buttons: {
		"Create": function() {
			$( "#add_edit_record_structure_form" ).submit();
		},
		Cancel: function() {
		  $( "#add_edit_record_structure_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
		// form[ 0 ].reset();
		// allFields.removeClass( "ui-state-error" );
		}
	});

	$('#add_edit_record_structure_form select#type').change(function() {

		switch($(this).val()) {
			case 'int':
			case 'timestamp':
			case 'boolean':
				$('#add_edit_record_structure_form input#length').val('');
				$('#add_edit_record_structure_form input#length').prop('readonly','readonly');
				$('#add_edit_record_structure_form input#dec_places').val('');
				$('#add_edit_record_structure_form input#dec_places').prop('readonly','readonly');
				break;
			case 'double':
				$('#add_edit_record_structure_form input#length').val('');
				$('#add_edit_record_structure_form input#length').prop('readonly','readonly');
				$('#add_edit_record_structure_form input#dec_places').prop('readonly','');
				break;
			case 'text':
				$('#add_edit_record_structure_form input#length').prop('readonly','');
				$('#add_edit_record_structure_form input#dec_places').val('');
				$('#add_edit_record_structure_form input#dec_places').prop('readonly','readonly');
				break;
		}
	});

	if(settings['form-errors'] != undefined) {
		$( "#add_edit_record_structure_form select#type" ).val(settings['form-values']['type']);
		$( "#add_edit_record_structure_form select#type" ).trigger('change');
		$( "#add_edit_record_structure_form input#title" ).val(settings['form-values']['title']);
		$( "#add_edit_record_structure_form input#length" ).val(settings['form-values']['length']);
		$( "#add_edit_record_structure_form input#dec_places" ).val(settings['form-values']['dec_places']);

		$( "#add_edit_record_structure_form_dialog p.error" ).html( settings['form-errors'].join("<br>") );

		$( "#add_edit_record_structure_form_dialog" ).dialog('open');
	}

});