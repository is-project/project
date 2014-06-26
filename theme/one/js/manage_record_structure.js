$(document).ready(function () {
  
  $('#example').handsontable({
    data: data,
    minSpareRows: 1,
    columns: [
      {
        type: 'text',
        data: 1
      },
      {
        type: 'dropdown',
        source: ["int", "double", "timestamp", "boolean", "text"],
        data: 2
      },
      {
        type: 'numeric',
        renderer: lengthRenderer,
        data: 3
      },
      {
        type: 'numeric',
        renderer: decimalPlacesRenderer,
        data: 4
      },
      {
        type: 'text',
        // renderer: decimalPlacesRenderer,
        data: 5
      },
      {
        type: 'text',
        data: "weight",
        renderer: sortRenderer,
        readOnly: true,
        data: 6
      }
    ],
    colHeaders: true, 
    colHeaders : ['display', 'type', 'length', 'decimal places', 'default value', 'order'],
    rowHeaders: true, 
    // contextMenu: ["row_above", "row_below", "hsep1", "remove_row", "hsep3", "undo", "redo"],
    stretchH: 'all',
    afterChange:  function(changes, source) {
                    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
                  },
    afterRender: function(isForeced) {
                    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
                  },
    cells: function (row, col, prop) {
        var cellProperties = {};

        var readOnly = false;
        var name = $('#example').handsontable('getData')[row][0];
        var systemNames = ['record', 'deleted', 'deleted_by', 'created', 'created_by'];
        if( $.inArray(name, systemNames) !== -1 )
          readOnly = true;

        cellProperties.isActive = true;
        if( col == 2 &&  $('#example').handsontable('getData')[row][2] !== 'text' ) {
          readOnly = true;
          cellProperties.isActive = false;
        }

        if( col == 3 &&  $('#example').handsontable('getData')[row][2] !== 'double' ) {
          readOnly = true;
          cellProperties.isActive = false;
        }

        cellProperties.readOnly = readOnly;

        return cellProperties;
      }

  });

  MathJax.Hub.Register.StartupHook("End",function () {
    $('#example').handsontable('selectCell',0,0);
    $('#example').handsontable('deselectCell');
  });  

});

var data = [
    ["record", "Record", "int", undefined, undefined, "auto_increment"],
    ["deleted", "Deleted", "timestamp", undefined, undefined, "NULL"],
    ["deleted_by", "Deleted By", "int", undefined, undefined, "NULL"],
    ["created", "Created", "timestamp", undefined, undefined, "NOW()"],
    ["created_by", "Created By", "int", undefined, undefined, "NULL"],
    ["param1", "$ p ~~\\mbox{in}~~ \\frac{\\mu C}{m^2K} $", "double"],
    ["param1", "$ T ~~\\mbox{in}~~ \\frac{\\mu C}{m^2K} $", "text"],
    ["param1", "$ d ~~\\mbox{in}~~ \\frac{\\mu C}{m^2K} $", "int"],
  ];

function decimalPlacesRenderer(instance, td, row, col, prop, value, cellProperties) {
  Handsontable.renderers.TextRenderer.apply(this, arguments);
  
  if(cellProperties.isActive === false) {
    td.style.background = '#EEE';
    $(td).html("");
  }

}

function lengthRenderer(instance, td, row, col, prop, value, cellProperties) {
  Handsontable.renderers.TextRenderer.apply(this, arguments);
  
  if(cellProperties.isActive === false) {
    td.style.background = '#EEE';
    $(td).html("");
  }

}

var sortRenderer = function (instance, td, row, col, prop, value, cellProperties) {

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
    instance.selectCell(0,0);
    instance.deselectCell();

  });

  $(td).empty().append($html); //empty is needed because you are rendering to an existing cell

  $(td).on('mousedown', function (event) {
    instance.deselectCell();
    event.preventDefault(); //prevent selection quirk
    event.stopPropagation();
    window.event.cancelBubble = true;
  });

  return td;
};