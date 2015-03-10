$(document).ready(function() {

  $( "#add_permissions_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Add Permission": function() {
      $( "#add_permissions_form" ).submit();
    },
    Cancel: function() {
      $( "#add_permissions_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
    // form[ 0 ].reset();
    // allFields.removeClass( "ui-state-error" );
    }
  });

  $( "#delete_permissions_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Delete": function() {
      $( "#delete_permissions_form" ).submit();
    },
    Cancel: function() {
      $( "#delete_permissions_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
    // form[ 0 ].reset();
    // allFields.removeClass( "ui-state-error" );
    }
  });


  $( "a#addPermissionsButton" ).click(function(){
    $( "#add_permissions_form_dialog" ).dialog('open');
  });



  $( "a#deletePermissionsButton" ).click(function(){
    if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
      alert('There has to be at least one selected element to delete.');

    if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
      var rows = new Array();
      $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
        rows[ rows.length ] = $(this).attr('id');
      });
      $( "#delete_permissions_form input#rows" ).val( JSON.stringify(rows) );
      
      $( "#delete_permissions_form_dialog" ).dialog('open');
    }
  });


});