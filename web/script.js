// ===== Helper Functions ===== //
function log(msg) {
  console.log(msg);
}

function E(id) {
  return document.getElementById(id);
}

function download_txt(fileName, fileContent) {
  var element = document.createElement('a');
  element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(fileContent));
  element.setAttribute('download', fileName);

  element.style.display = 'none';
  document.body.appendChild(element);

  element.click();

  document.body.removeChild(element);
}

function fixFileName(fileName) {
  if (fileName.length > 0 && fileName[0] != '/') {
    fileName = '/' + fileName;
  }

  fileName = fileName.replace(/ /g, '\-');

  return fileName;
}

// ===== Value Getters ===== //
function getNewFileName() {
  return E("newFile").value;
}

function getEditorFileName() {
  return E("editorFile").value;
}

function getEditorContent() {
  return E("editor").value;
}

// ===== DOM Manipulation ===== //
function status(mode) {
  if (mode == "connected") {
    E("status").style.backgroundColor = "#3c5";
  } else if (mode == "disconnected") {
    E("status").style.backgroundColor = "#d33";
  } else if (mode == "connecting...") {
    E("status").style.backgroundColor = "#0ae";
  }

  E("status").innerHTML = mode;
}

// ===== Web Socket ===== //
function log_ws(msg) {
  log("[WS] " + msg);
}

var ws; // web socket instance
var ws_callback = log_ws; // message receive callback
var ws_msg_queue = []; // queue for outgoing messages
var cts = false; // clear to send flag for message queue

var file_list = ""; // file list string used for ls command

// ===== WebSocket Functions ===== //
function ws_msg_queue_update() {
  if (cts && ws_msg_queue.length > 0) {
    var item = ws_msg_queue.shift();

    var message = item.message;
    var callback = item.callback;

    ws.send(message);
    ws_callback = callback;

    console.debug("# " + message);

    cts = false;
  }
}

function ws_init() {
  status("connecting...");

  ws = new WebSocket("ws://192.168.4.1/ws");

  ws.onopen = function(event) {
    log_ws("connected");
    status("connected");

    ws_send_mem_ls();
  };

  ws.onclose = function(event) {
    log_ws("disconnected");
    status("disconnected");
  };

  ws.onmessage = function(event) {
    var msg = event.data;

    log_ws(msg);

    if (msg.length > 0) {
      ws_callback(msg);
    }

    cts = true;
  };

  ws.onerror = function(event) {
    log_ws("error");
    status("error");

    console.error(event);
  };

  cts = true;
}

function ws_send(message, callback) {
  ws_msg_queue.push({
    "message": message,
    "callback": callback
  });
}

// ===== WebSocket Actions ===== //
function ws_send_format() {
  if (confirm("Format SPIFFS? This will delete all scripts!")) {
    ws_send("format", log_ws);
    alert("Formatting will take a minute. You have to reconnect afterwards.");
  }
}

function ws_send_run(fileName) {
  ws_send("run \"" + fixFileName(fileName) + "\"", log_ws);
}

function ws_send_stop(fileName) {
  ws_send("stop \"" + fixFileName(fileName) + "\"", log_ws);
}

function ws_send_stream(fileName) {
  ws_send_stop(fileName);

  fileName = fixFileName(fileName);

  E("editorFile").value = fileName;
  E("editor").value = "";

  ws_send("stream \"" + fileName + "\"\n", log_ws);

  var ws_send_read = function() {
    ws_send("read", function(content) {
      if (content != "> END") {
        E("editor").value += content;
        ws_send_read();
      } else {
        ws_send("close", log_ws);
      }
    });
  };

  ws_send_read();
}

function ws_send_ls() {
  file_list = "";

  ws_send("ls\n", function(csv) {
    file_list += csv;

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
      var data = lines[i].split(" ");
      var fileName = data[0];
      var fileSize = data[1];
      if (fileName.length > 0) {
        tableHTML += "<tr>\r\n";
        tableHTML += "<td>" + fileName + "</td>\r\n";
        tableHTML += "<td>" + fileSize + "</td>\r\n";
        tableHTML += "<td>\r\n";
        tableHTML += "<button class=\"primary\" onclick=\"ws_send_stream('" + fileName + "')\">edit</button>\r\n";
        tableHTML += "<button class=\"warn\" onclick=\"ws_send_run('" + fileName + "')\">run</button></td>\r\n";
        tableHTML += "</tr>\r\n";
      }
    }

    tableHTML += "<tr>\r\n";
    tableHTML += "<td><input type=\"text\" class=\"smooth\" value=\"/\" id=\"newFile\"/></td>\r\n";
    tableHTML += "<td>-</td>\r\n";
    tableHTML += "<td><button class=\"success\" onclick=\"ws_send_create(getNewFileName())\">create</button></td>\r\n";
    tableHTML += "</tr>\r\n";
    tableHTML += "</tbody>\r\n";

    E("scriptTable").innerHTML = tableHTML;
  });
}

function ws_send_mem_ls() {
  ws_send("mem", function(msg) {
    var lines = msg.split(/\n/);
    var byte = lines[0].split(" ")[0];
    var used = lines[1].split(" ")[0];
    var free = lines[2].split(" ")[0];

    var percent = Math.floor(byte / 100);
    var freepercent = Math.floor(free / percent);

    E("freeMemory").innerHTML = used + " byte used (" + freepercent + "% free)";
    ws_send_ls();
  });
}

function ws_send_create(fileName) {
  ws_send_stop(fileName);

  fileName = fixFileName(fileName);

  E("editorFile").value = fileName;
  E("editor").value = "";

  ws_send("create \"" + fileName + "\"\n", log_ws);
}

function ws_send_write(fileName, content) {
  ws_send_stop(fileName);

  fileName = fixFileName(fileName);
  content = content.replace(/"/g, '\\"');

  ws_send("remove \"/temporary_script\"", log_ws);
  ws_send("create \"/temporary_script\"", log_ws);

  ws_send("stream \"/temporary_script\"", log_ws);

  var pktsize = 64;
  for (var i = 0; i < Math.ceil(content.length / pktsize); i++) {
    var begin = i * pktsize;
    var end = begin + pktsize;
    if (end > content.length) end = content.length;

    ws_send(content.substring(begin, end), log_ws);
  }
  ws_send("close", log_ws);

  ws_send("remove \"" + fileName + "\"", log_ws);
  ws_send("rename \"/temporary_script\" \"" + fileName + "\"", log_ws);

  ws_send_mem_ls();
}

function ws_send_remove(fileName) {
  ws_send_stop(fileName);

  ws_send("remove \"" + fixFileName(fileName) + "\"", log_ws);

  ws_send_mem_ls();
}

// ===== Startup ===== //
window.addEventListener("load", function() {

  E("scriptsReload").onclick = function() {
    ws_send_mem_ls();
  };

  E("reconnect").onclick = function() {
    ws_init();
  };

  E("format").onclick = function() {
    ws_send_format();
  };

  E("stop").onclick = function() {
    ws_send_stop("");
  };

  E("editorReload").onclick = function() {
    ws_send_stream(getEditorFileName());
  };

  E("editorSave").onclick = function() {
    ws_send_write(getEditorFileName(), getEditorContent());
  };

  E("editorDelete").onclick = function() {
    if (confirm("Delete " + getEditorFileName() + "?")) {
      ws_send_remove(getEditorFileName());
    }
  };

  E("editorDownload").onclick = function() {
    download_txt(getEditorFileName(), getEditorContent());
  };

  E("editorStop").onclick = function() {
    ws_send_stop(getEditorFileName());
  }

  E("editorRun").onclick = function() {
    ws_send_run(getEditorFileName());
  };

  ws_init();

  setInterval(ws_msg_queue_update, 1);
}, false);