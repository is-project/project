$(document).ready(function() {

  $( "#add_edit_groups_form_dialog" ).dialog({
    autoOpen: false,
    height: 350,
    width: 400,
    modal: true,
    buttons: {
    "Submit": function() {
      $( "#add_edit_groups_form" ).submit();
    },
    Cancel: function() {
      $( "#add_edit_groups_form_dialog" ).dialog( "close" );
    }
    },
    close: function() {
      $( "#add_edit_groups_form input#group-name" ).val('');
      $( "#add_edit_groups_form input#group" ).val('');
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

      $( "#add_edit_groups_form input#group-name" ).val( name );
      $( "#add_edit_groups_form input#group" ).val( id );

      $( "#add_edit_groups_form_dialog" ).dialog('open');
    }

  });

  $( "a#deleteGroupButton" ).click(function(){
    if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length == 0)
      alert('There has to be at least one selected element to delete.');

    if( $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).length >= 1) {
      var rows = new Array();
      $( 'table.data input[type=checkbox]:checked:not(#checkall)' ).each(function(){
        rows[ rows.length ] = $(this).attr('id');
      });
      $( "#delete_groups_form input#rows" ).val( JSON.stringify(rows) );

      $( "#delete_groups_form_dialog" ).dialog('open');
    }
  });




});