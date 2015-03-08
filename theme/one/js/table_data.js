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

});