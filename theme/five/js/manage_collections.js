$(document).ready(function () {

	$( "#add_edit_collection_form_dialog" ).dialog({
		autoOpen: false,
		height: 450,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#add_edit_collection_form" ).submit();
		},
		Cancel: function() {
			$( "#add_edit_collection_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#add_edit_collection_form input#collection" ).val('');
			$( "#add_edit_collection_form input#name" ).val('');
			$( "#add_edit_collection_form textarea#description" ).val('');

			$( "#add_edit_collection_form_dialog p.error" ).html( '' );
		}
	});

	$( "#delete_collection_form_dialog" ).dialog({
		autoOpen: false,
		height: 250,
		width: 400,
		modal: true,
		buttons: {
		"Submit": function() {
			$( "#delete_collection_form" ).submit();
		},
		Cancel: function() {
			$( "#delete_collection_form_dialog" ).dialog( "close" );
		}
		},
		close: function() {
			$( "#add_edit_collection_form input#collections" ).val('');

			$( "#add_edit_collection_form_dialog p.error" ).html( '' );
		}
	});

	$( "a#addCollectionButton" ).click(function(){
		$( "#add_edit_collection_form_dialog" ).dialog('open');
	});

	$( "a#editCollectionButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be one selected element to edit.');
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length > 1)
			alert('Only one element has to be selected to edit.');

		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 1) {

			var id = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).attr('id');

			var name = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child(2)').html()
			name = $("<div>"+name+"</div>")
			$(name).find('>script').each(function() {
				$(this).html("$" + $(this).html() + "$");
			});
			$(name).find('>span').html('');
			name = $(name).text();

			var description = $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).closest('tr').find('td:nth-child(3)').html()
			description = $("<div>"+description+"</div>")
			$(description).find('>script').each(function() {
				$(this).html("$" + $(this).html() + "$");
			});
			$(description).find('>span').html('');
			description = $(description).text();

			$( "#add_edit_collection_form input#name" ).val( name );
			$( "#add_edit_collection_form textarea#description" ).val( description );
			$( "#add_edit_collection_form input#collection" ).val( id );

			$( "#add_edit_collection_form_dialog" ).dialog('open');
		}

	});

	$( "a#deleteCollectionButton" ).click(function(){
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
			alert('There has to be at least one selected element to delete.');
		
		if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
			var collections = new Array();
			$( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
				collections[ collections.length ] = $(this).attr('id');
			});
			$( "#delete_collection_form input#collections" ).val( JSON.stringify(collections) );
			// console.log( cols );
			$( "#delete_collection_form_dialog" ).dialog('open');
		}
	});

	if(settings['form-errors'] != undefined) {
		$( "#add_edit_collection_form input#name" ).val(settings['form-values']['name']);
		$( "#add_edit_collection_form textarea#description" ).val(settings['form-values']['description']);
		$( "#add_edit_collection_form input#collection" ).val(settings['form-values']['collection']);

		$( "#add_edit_collection_form_dialog p.error" ).html( settings['form-errors'].join("<br>") );

		$( "#add_edit_collection_form_dialog" ).dialog('open');
	}

});