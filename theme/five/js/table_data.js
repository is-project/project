$(document).ready(function() {
	
	$( 'table.data input#checkall' ).click(function() {
		$( 'table.data input[type=checkbox]:not(#checkall)' ).prop('checked', $(this).prop('checked') );
	});

	$( 'table.data input[type=checkbox]:not(#checkall)' ).click(function() {
		var checked = $(this).prop('checked');
		var same = true;
		$( 'table.data input[type=checkbox]:not(#checkall)' ).each(function() {
			if( $(this).prop('checked') != checked ) same = false;
		});
		$( 'table.data input#checkall' ).prop('checked', same&&checked);
	});

	var fixHelper = function(e, ui) {
		ui.children().each(function() {
			$(this).width($(this).width());
		});
		return ui;
	};

	$("table.data tbody").sortable({
		helper: fixHelper,
		handle : '.sortHandle',
		update: function( event, ui ) {
			$('.tipChangesNotSaved').show();
		}
    }).disableSelection();

	$('a#orderButton').click(function() {
		var order = new Array();
		$('table.data tbody tr td:nth-child(1) input[type=checkbox]').each(function(){
			order[order.length] = $(this).attr('id');
		});
		$('form#orderForm input#order').val( JSON.stringify(order) );

		$( "form#orderForm" ).submit();
	});

});