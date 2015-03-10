$(document).ready(function() {

  $( "#add_members_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Create": function() {
      $( "#add_members_form" ).submit();
    },
    Cancel: function() {
      $( "#add_members_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
    // form[ 0 ].reset();
    // allFields.removeClass( "ui-state-error" );
    }
  });

  $( "#delete_members_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Delete": function() {
      $( "#delete_members_form" ).submit();
    },
    Cancel: function() {
      $( "#delete_mebmers_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
    // form[ 0 ].reset();
    // allFields.removeClass( "ui-state-error" );
    }
  });


  $( "a#addMembersButton" ).click(function(){
    $( "#add_members_form_dialog" ).dialog('open');
  });



  $( "a#deleteMembersButton" ).click(function(){
    if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
      alert('There has to be at least one selected element to delete.');

    if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
      var rows = new Array();
      $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
        rows[ rows.length ] = $(this).attr('id');
      });
      $( "#delete_members_form input#rows" ).val( JSON.stringify(rows) );
      
      $( "#delete_members_form_dialog" ).dialog('open');
    }
  });


});