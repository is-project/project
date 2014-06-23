$(document).ready(function () {

  var data = [
    ["record", "Record", "int"],
    ["deleted", "Deleted", "timestamp"],
    ["deleted_by", "Deleted By", "int"],
    ["created", "Created Lorem Ipsum adsjfnbsdf sdf sdf sdf sdfg", "timestamp"],
    ["created by", "Created By", "int"],
  ];

  var sortRenderer = function (instance, td, row, col, prop, value, cellProperties) {
    console.log(instance.countRows());

    if( instance.isEmptyRow(row) ) return $(td).empty().append('');

    var escaped = Handsontable.helper.stringify(value);
    var $html = $('<a class="moveBtn up"><span class="ui-icon ui-icon-arrowthick-1-n" style="display: inline-block;"></span><span style="display: inline-block; overflow: hidden;">up</span></a> | <a class="moveBtn down"><span class="ui-icon ui-icon-arrowthick-1-s" style="display: inline-block;"></span><span style="display: inline-block; overflow: hidden;">down</span></a>');
    
    if(row == 0) $($html[0]).addClass('disabled');
    if(row == instance.countRows()-2) $($html[2]).addClass('disabled');
    
    $html.click(function() {
      if($(this).is(".disabled")) return;

      var $rows = $(this).closest("tbody").find('tr');
      var $row = $(this).closest('tr');
      var cols = instance.getSettings().columns;

      if ($(this).is(".up")) {
        var rowUp = $rows.index($row);
        var rowDown = $rows.index($row.prev());
        $row.insertBefore($row.prev());
      } else {
        var rowDown = $rows.index($row);
        var rowUp = $rows.index($row.next());
        $row.insertAfter($row.next());
      }

      var dataRowDown = data[rowDown];
      var dataRowUp = data[rowUp]; 
      data[rowUp] = dataRowDown;
      data[rowDown] = dataRowUp;

      for (var i=0; i<cols.length; i++) {
        var rowUpValid = instance.getCellMeta(rowUp, i).valid;
        var rowDownValid = instance.getCellMeta(rowDown, i).valid;
        instance.getCellMeta(rowUp, i).valid = rowDownValid;
        instance.getCellMeta(rowDown, i).valid = rowUpValid;
      }

      instance.forceFullRender = true;
      instance.view.render(); //updates all

    });

    $(td).empty().append($html); //empty is needed because you are rendering to an existing cell

    $(td).on('mousedown', function (event) {
      event.preventDefault(); //prevent selection quirk
      event.stopPropagation();
      window.event.cancelBubble = true;
    });

    return td;
  };
  
  $('#example').handsontable({
    data: data,
    minSpareRows: 1,
    columns: [
      { type: 'text' },
      { type: 'text' },
      {
        type: 'dropdown',
        source: ["int", "double", "timestamp", "boolean", "text"]
      },
      {
        type: 'numeric',
        readOnly: true,
      },
      {
        type: 'numeric',
        readOnly: true,
      },
      {
        type: 'text',
        data: "weight",
        renderer: sortRenderer,
        readOnly: true,
      }
    ],
    colHeaders: true, 
    colHeaders : ['name', 'display', 'type', 'length', 'decimal places', 'order'],
    rowHeaders: true, 
    contextMenu: ["row_above", "row_below", "hsep1", "remove_row", "hsep3", "undo", "redo"],
    //width: 1000,
    
    stretchH: 'all',
    autoWrapRow: true,

  });

  // $('#example').handsontable('getData');

  

});