$(document).ready(function () {

	$( "#add_edit_record_structure_form_dialog" ).dialog({
		autoOpen: false,
		height: 350,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#add_edit_record_structure_form" ).submit();
		},
		Cancel: function() {
			$( "#add_edit_record_structure_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#add_edit_record_structure_form select#type" ).val('int');
			$( "#add_edit_record_structure_form select#type" ).trigger('change');
			$( "#add_edit_record_structure_form input#title" ).val('');
			$( "#add_edit_record_structure_form input#col" ).val('');

			$( "#add_edit_record_structure_form_dialog p.error" ).html( '' );
		}
	});

	$( "#delete_record_structure_form_dialog" ).dialog({
		autoOpen: false,
		height: 250,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#delete_record_structure_form" ).submit();
		},
		Cancel: function() {
			$( "#delete_record_structure_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#add_edit_record_structure_form input#cols" ).val('');

			$( "#add_edit_record_structure_form_dialog p.error" ).html( '' );
		}
	});

	$( "a#addRecordStructureButton" ).click(function(){
		$( "#add_edit_record_structure_form_dialog" ).dialog('open');
	});

	$( "a#editRecordStructureButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be one selected element to edit.');
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length > 1)
			alert('Only one element has to be selected to edit.');

		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 1) {

			var id = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).attr('id');

			var title = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child(2)').html()
			title = $("<div>"+title+"</div>")
			$(title).find('>script').each(function() {
				$(this).html("$" + $(this).html() + "$");
			});
			$(title).find('>span').html('');
			title = $(title).text();

			var type = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child(3)').html();
			var length = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child(4)').html();
			var dec_places = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child(5)').html();

			$( "#add_edit_record_structure_form select#type" ).val( type );
			$( "#add_edit_record_structure_form select#type" ).trigger('change');
			$( "#add_edit_record_structure_form input#title" ).val( title );
			$( "#add_edit_record_structure_form input#length" ).val( length );
			$( "#add_edit_record_structure_form input#dec_places" ).val( dec_places );
			$( "#add_edit_record_structure_form input#col" ).val( id );

			$( "#add_edit_record_structure_form_dialog" ).dialog('open');
		}

	});

	$( "a#deleteRecordStructureButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be at least one selected element to delete.');
		
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
			var cols = new Array();
			$( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
				cols[ cols.length ] = $(this).attr('id');
			});
			$( "#delete_record_structure_form input#cols" ).val( JSON.stringify(cols) );
			// console.log( cols );
			$( "#delete_record_structure_form_dialog" ).dialog('open');
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
		$( "#add_edit_record_structure_form input#col" ).val(settings['form-values']['col']);

		$( "#add_edit_record_structure_form_dialog p.error" ).html( settings['form-errors'].join("<br>") );

		$( "#add_edit_record_structure_form_dialog" ).dialog('open');
	}

});