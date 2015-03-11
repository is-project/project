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

	$( "#delete_record_form_dialog" ).dialog({
		autoOpen: false,
		height: 250,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#delete_record_form" ).submit();
		},
		Cancel: function() {
			$( "#delete_record_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#delete_record_form input#records" ).val('');
		}
	});

	$( "#add_record_to_collection_form_dialog" ).dialog({
		autoOpen: false,
		height: 300,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#add_record_to_collection_form" ).submit();
		},
		Cancel: function() {
			$( "#add_record_to_collection_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#add_record_to_collection_form input#records" ).val('');
		}
	});

	$( "a#addRecordButton" ).click(function(){
		$( "#add_edit_record_form_dialog" ).dialog('open');
	});

	$( "a#editRecordButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be one selected element to edit.');
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length > 1)
			alert('Only one element has to be selected to edit.');

		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 1) {

			var id = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).attr('id');
			$( "#add_edit_record_form input#record" ).val( id );

			var i = 2;
			for (var key in settings.record_structure) {
				var val = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child('+i+')').html()
				val = $("<div>"+val+"</div>");
				if($(val).find('span.ui-icon').length) { // bool
					if($(val).find('span.ui-icon.ui-icon-check').length)
						$( "#add_edit_record_form input#"+key ).prop('checked','checked');
					else
						$( "#add_edit_record_form input#"+key ).prop('checked','');
				} else {
					$(val).find('>script').each(function() {
						$(this).html("$" + $(this).html() + "$");
					});
					$(val).find('>span').html('');
					val = $(val).text();
					if(val == '-') val = '';

					$( "#add_edit_record_form input#"+key ).val( val );
				}
				console.log(key + ' -> ' + val);

				i++;
			}

			$( "#add_edit_record_form_dialog" ).dialog('open');
		}
	});

	$( "a#deleteRecordButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be at least one selected element to delete.');
		
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
			var records = new Array();
			$( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
				records[ records.length ] = $(this).attr('id');
			});
			$( "#delete_record_form input#records" ).val( JSON.stringify(records) );
			// console.log( cols );
			$( "#delete_record_form_dialog" ).dialog('open');
		}
	});

	$( "a#addRecordToCollectionButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be at least one selected element to delete.');
		
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
			var records = new Array();
			$( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
				records[ records.length ] = $(this).attr('id');
			});
			$( "#add_record_to_collection_form input#records" ).val( JSON.stringify(records) );
			// console.log( cols );
			$( "#add_record_to_collection_form_dialog" ).dialog('open');
		}
	});


});