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


  $('#add_edit_groups_form select#type').change(function() {

    switch($(this).val()) {
      case 'int':
      case 'timestamp':
      case 'boolean':
        $('#add_edit_groups_form input#length').val('');
        $('#add_edit_groups_form input#length').prop('readonly','readonly');
        $('#add_edit_groups_form input#dec_places').val('');
        $('#add_edit_groups_form input#dec_places').prop('readonly','readonly');
        break;
      case 'double':
        $('#add_edit_groups_form input#length').val('');
        $('#add_edit_groups_form input#length').prop('readonly','readonly');
        $('#add_edit_groups_form input#dec_places').prop('readonly','');
        break;
      case 'text':
        $('#add_edit_groups_form input#length').prop('readonly','');
        $('#add_edit_groups_form input#dec_places').val('');
        $('#add_edit_groups_form input#dec_places').prop('readonly','readonly');
        break;
    }
  });

});