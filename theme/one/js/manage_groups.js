$(document).ready(function() {

  $( "#add_edit_groups_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Create": function() {
      $( "#add_edit_groups_form" ).submit();
    },
    Cancel: function() {
      $( "#add_edit_groups_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
    // form[ 0 ].reset();
    // allFields.removeClass( "ui-state-error" );
    }
  });

  $( "#delete_groups_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Delete": function() {
      $( "#delete_groups_form" ).submit();
    },
    Cancel: function() {
      $( "#delete_groups_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
    // form[ 0 ].reset();
    // allFields.removeClass( "ui-state-error" );
    }
  });


  $( "a#addGroupButton" ).click(function(){
    $( "#add_edit_groups_form_dialog" ).dialog('open');
  });



  $( "a#editGroupButton" ).click(function(){
    //$( "#add_edit_record_structure_form_dialog" ).dialog('open');

    alert( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length );
  });

  $( "a#deleteGroupButton" ).click(function(){
    $( "#delete_groups_form_dialog" ).dialog('open');

  });




});