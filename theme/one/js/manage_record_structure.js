$(document).ready(function () {

  $('#example').handsontable({
    data: settings.data.length ? settings.data : undefined,
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
    colHeaders : ['name', 'type', 'length', 'displayed decimal places', 'default value', 'order'],
    rowHeaders: true, 
    contextMenu: ["row_above", "row_below", "hsep1", "remove_row", "hsep3", "undo", "redo"],
    stretchH: 'all',
    afterChange:  function(changes, source) {
                    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
                    MathJax.Hub.Queue(function() {
                      var tmp = $('#example').handsontable('getSelected');
                      if(tmp != undefined) {
                        $('#example').handsontable('deselectCell');
                        $('#example').handsontable('selectCell', tmp[0], tmp[1]);
                      }
                    });
                    $('input#record-structure').val( JSON.stringify($('#example').handsontable('getData')) );
                  },
    afterRender: function(isForeced) {
                    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
                  },
    cells: cellProcess

  });

  MathJax.Hub.Register.StartupHook("End",function () {
    $('#example').handsontable('selectCell',0,0);
    $('#example').handsontable('deselectCell');
  });

  $('input#record-structure').val( JSON.stringify($('#example').handsontable('getData')) );

});

var data = JSON.parse(JSON.stringify(settings.data));

function cellProcess(row, col, prop) {
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

function decimalPlacesRenderer(instance, td, row, col, prop, value, cellProperties) {
  Handsontable.renderers.TextRenderer.apply(this, arguments);
  
  if(cellProperties.isActive === false) {
    td.style.background = '#EEE';
    $(td).html("");
  }

  if(!instance.isEmptyRow(row)) {
    if( settings.data[row] != undefined && settings.data[row].equals( data[row] ) ) {
      $(td).closest('tr').removeClass('changed').addClass('saved');
    } else {
      $(td).closest('tr').removeClass('saved').addClass('changed');
    }
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

    var dataRowDown = settings.data[rowDown];
    var dataRowUp = settings.data[rowUp]; 
    settings.data[rowUp] = dataRowDown;
    settings.data[rowDown] = dataRowUp;

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

  $('input#record-structure').val( JSON.stringify($('#example').handsontable('getData')) );

  return td;
};

Array.prototype.equals = function (array) {
    // if the other array is a falsy value, return
    if (!array)
        return false;

    // compare lengths - can save a lot of time 
    if (this.length != array.length)
        return false;

    for (var i = 0, l=this.length; i < l; i++) {
        // Check if we have nested arrays
        if (this[i] instanceof Array && array[i] instanceof Array) {
            // recurse into the nested arrays
            if (!this[i].equals(array[i]))
                return false;       
        }           
        else if (this[i] != array[i]) { 
            // Warning - two different object instances will never be equal: {x:20} != {x:20}
            return false;   
        }           
    }       
    return true;
}