$(document).ready(function () {

  var data = [
    ["record", "Record", "int"],
    ["deleted", "Deleted", "timestamp", "dfghjkdfghjkdfghjkdfghjkdfghjkdfghjkdfghjkdfghjkdfghjkdfghjk"],
    ["deleted_by", "Deleted By", "int"],
    ["created", "Created", "timestamp"],
    ["created by", "Created By", "int"],
  ];
  
  $('#example').handsontable({
    data: data,
    minSpareRows: 1,
    columns: [
      { type: 'text' },
      { type: 'text' },
      {
        type: 'dropdown',
        source: ["int", "double", "timestamp", "boolean"]
      }
    ],
    colHeaders: true, 
    colHeaders : ['name', 'display', 'type'],    
    rowHeaders: true, 
    contextMenu: ["row_above", "row_below", "hsep1", "remove_row", "hsep3", "undo", "redo"]
  });

  // $('#example').handsontable('getData');

});