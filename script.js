function status(mode) {
  if (mode == "connected") {
    document.getElementById("status").style.backgroundColor = "#3c5";
  } else if (mode == "disconnected") {
    document.getElementById("status").style.backgroundColor = "#d33";
  } else if (mode == "connecting...") {
    document.getElementById("status").style.backgroundColor = "#0ae";
  }

  document.getElementById("status").innerHTML = mode;
}

function log(msg) {
  console.log("[WS] Message: " + msg);
}

var ws;
var ws_callback = log;
var file_list = "";
var messageQueue = [];
var cts = false;

// ===== Helper Functions ===== //
function getNewFileName() {
  return document.getElementById("newFile").value;
}

function getEditorFileName() {
  return document.getElementById("editorFile").value;
}

function getEditorContent() {
  return document.getElementById("editor").value;
}

function fixFileName(fileName) {
  if (fileName.length > 0 && fileName[0] != '/') fileName = '/' + fileName;
  fileName = fileName.replace(/ /g, '\-');
  return fileName;
}

// ===== WebSocket Functions ===== //
function ws_init() {
  status("connecting...");
  ws = new WebSocket("ws://192.168.4.1/ws");
  ws.onopen = ws_open;
  ws.onclose = ws_close;
  ws.onmessage = ws_message;
  ws.onerror = ws_error;

  cts = true;
}

function ws_open(evt) {
  console.log("[WS] Connected");
  status("connected");
  ws_memory(ws_ls);
}

function ws_close(evt) {
  console.log("[WS] Disconnected");
  status("disconnected");
}

function ws_message(evt) {
  var msg = evt.data;

  console.debug("> " + evt.data);

  if (msg.length > 0) {
    if (msg[0] == '>') log(msg.substring(2));
    ws_callback(msg);
  }
  cts = true;
}

function ws_error(evt) {
  console.log("[WS] Error");
  status("errored");
}

function ws_send(message, callback) {
  messageQueue.push({
    "message": message,
    "callback": callback
  });
}

// ===== WebSocket Actions ===== //
function ws_memory(callback) {
  ws_send("mem", function(msg) {
    var lines = msg.split(/\n/);
    var byte = lines[0].split(" ")[0];
    var used = lines[1].split(" ")[0];
    var free = lines[2].split(" ")[0];

    var percent = Math.floor(byte / 100);
    var freepercent = Math.floor(free / percent);

    document.getElementById("freeMemory").innerHTML = used + " byte used (" + freepercent + "% free)";
    if (callback) callback();
  });
}

function ws_run(fileName) {
  fileName = fixFileName(fileName);
  ws_send("run \"" + fileName + "\"", log);
}

function ws_format() {
  if (confirm("Format SPIFFS? This will delete all scripts!")) {
    ws_send("format", log);
    alert("Formatting will take a minute. You need to reconnect afterwards.");
  }
}

function ws_stop(fileName) {
  fileName = fixFileName(fileName);
  ws_send("stop \"" + fileName + "\"", log);
}

function ws_ls() {
  file_list = "";
  ws_send("ls\n", function(csv) {
    file_list += csv;

    //console.log(file_list);

    var lines = file_list.split(/\n/);
    var tableHTML = "<thead>\r\n";

    tableHTML += "<tr>\r\n";
    tableHTML += "<th>File</th>\r\n";
    tableHTML += "<th>Size</th>\r\n";
    tableHTML += "<th>Actions</th>\r\n";
    tableHTML += "</tr>\r\n";
    tableHTML += "</thead>\r\n";
    tableHTML += "<tbody>\r\n";

    for (var i = 0; i < lines.length; i++) {
      //console.log(lines[i]);
      var data = lines[i].split(" ");
      var fileName = data[0];
      var fileSize = data[1];
      if (fileName.length > 0) {
        tableHTML += "<tr>\r\n";
        tableHTML += "<td>" + fileName + "</td>\r\n";
        tableHTML += "<td>" + fileSize + "</td>\r\n";
        tableHTML += "<td>\r\n";
        tableHTML += "<button class=\"primary\" onclick=\"ws_cat('" + fileName + "')\">edit</button>\r\n";
        tableHTML += "<button class=\"warn\" onclick=\"ws_run('" + fileName + "')\">run</button></td>\r\n";
        tableHTML += "</tr>\r\n";
      }
    }

    tableHTML += "<tr>\r\n";
    tableHTML += "<td><input type=\"text\" class=\"smooth\" value=\"/\" id=\"newFile\"/></td>\r\n";
    tableHTML += "<td>-</td>\r\n";
    tableHTML += "<td><button class=\"success\" onclick=\"ws_create(getNewFileName())\">create</button></td>\r\n";
    tableHTML += "</tr>\r\n";
    tableHTML += "</tbody>\r\n";

    document.getElementById("scriptTable").innerHTML = tableHTML;
  });
}

function ws_read() {
  ws_send("read", function(content) {
    if (content != "> END") {
      document.getElementById("editor").value += content;
      ws_read();
    } else {
      ws_send("close", log);
    }
  });
}

function ws_cat(fileName) {
  ws_stop(fileName);

  fileName = fixFileName(fileName);

  document.getElementById("editorFile").value = fileName;
  document.getElementById("editor").value = "";

  ws_send("stream \"" + fileName + "\"\n", log);
  ws_read();
}

function ws_create(fileName) {
  ws_stop(fileName);

  fileName = fixFileName(fileName);

  document.getElementById("editorFile").value = fileName;
  document.getElementById("editor").value = "";

  ws_send("create \"" + fileName + "\"\n", log);
}

function ws_write(fileName, content) {
  ws_stop(fileName);

  fileName = fixFileName(fileName);
  content = content.replace(/"/g, '\\"');

  ws_send("remove \"/temporary_script\"", log);
  ws_send("create \"/temporary_script\"", log);

  ws_send("stream \"/temporary_script\"", log);

  var pktsize = 64;
  for (var i = 0; i < Math.ceil(content.length / pktsize); i++) {
    var begin = i * pktsize;
    var end = begin + pktsize;
    if (end > content.length) end = content.length;

    ws_send(content.substring(begin, end), log);
  }
  ws_send("close", log);

  ws_send("remove \"" + fileName + "\"", log);
  ws_send("rename \"/temporary_script\" \"" + fileName + "\"", log);

  ws_ls();
}

function ws_remove(fileName) {
  ws_stop(fileName);

  fileName = fixFileName(fileName);
  ws_send("remove \"" + fileName + "\"", log);

  ws_ls();
}


// ===== Startup ===== //
window.addEventListener("load", function() {

  document.getElementById("scriptsReload").onclick = function() {
    ws_memory(ws_ls);
  };

  document.getElementById("reconnect").onclick = function() {
    ws_init();
  };

  document.getElementById("format").onclick = function() {
    ws_format();
  };

  document.getElementById("stop").onclick = function() {
    ws_stop("");
  };

  document.getElementById("editorReload").onclick = function() {
    ws_cat(getEditorFileName());
  };

  document.getElementById("editorSave").onclick = function() {
    ws_write(getEditorFileName(), getEditorContent());
  };

  document.getElementById("editorDelete").onclick = function() {
    if (confirm("Delete " + getEditorFileName() + "?")) ws_remove(getEditorFileName());
  };

  document.getElementById("editorDownload").onclick = function() {
    fileName = getEditorFileName();
    content = getEditorContent();

    var element = document.createElement('a');
    element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(content));
    element.setAttribute('download', fileName);

    element.style.display = 'none';
    document.body.appendChild(element);

    element.click();

    document.body.removeChild(element);
  };

  document.getElementById("editorStop").onclick = function() {
    ws_stop(getEditorFileName());
  }

  document.getElementById("editorRun").onclick = function() {
    ws_run(getEditorFileName());
  };

  ws_init();

  setInterval(function() {
    if (cts && messageQueue.length > 0) {
      var item = messageQueue.shift();

      var message = item.message;
      var callback = item.callback;

      ws.send(message);
      ws_callback = callback;

      console.debug("# " + message);

      cts = false;
    }

  }, 1);
}, false);
