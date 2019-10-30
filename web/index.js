/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

var file_list = ""; // file list string used for ls command
var status_interval = undefined;

function start_status_interval() {
  if (!status_interval) {
    ws_update_status();
    status_interval = setInterval(check_status, 200);
  }
}

function stop_status_interval() {
  if (status_interval) {
    clearInterval(status_interval);
    status_interval = undefined;
  }
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

// ===== WebSocket Actions ===== //
function check_status() {
  if (current_status.includes("running")) {
    ws_update_status();
  } else {
    stop_status_interval();
  }
}

function ws_connected() {
  ws_send_mem_ls();
}

function ws_send_format() {
  if (confirm("Format SPIFFS? This will delete all scripts!")) {
    ws_send("format", log_ws);
    alert("Formatting will take a minute. You have to reconnect afterwards.");
  }
}

function ws_send_run(fileName) {
  ws_send("run \"" + fixFileName(fileName) + "\"", log_ws);
  start_status_interval();
}

function ws_send_stop(fileName) {
  var cmd = "stop";

  if (fileName) {
    cmd += " \"" + fixFileName(fileName) + "\"";
  }

  stop_status_interval();

  ws_send(cmd, log_ws, true);
  ws_update_status();
}

function ws_send_read() {
  ws_send("read", function(content) {
    if (content != "> END") {
      E("editor").value += content;
      ws_send_read();
      status("reading...");
    } else {
      ws_send("close", log_ws);
      ws_update_status();
    }
  });
}

function ws_send_stream(fileName) {
  ws_send_stop(fileName);

  fileName = fixFileName(fileName);

  E("editorFile").value = fileName;
  E("editor").value = "";

  ws_send("stream \"" + fileName + "\"", log_ws);

  ws_send_read();
}

function ws_send_ls() {
  file_list = "";

  ws_send("ls", function(csv) {
    file_list += csv;

    var lines = file_list.split(/\n/);
    var tableHTML = "<thead>\n";

    tableHTML += "<tr>\n";
    tableHTML += "<th>File</th>\n";
    tableHTML += "<th>Size</th>\n";
    tableHTML += "<th>Actions</th>\n";
    tableHTML += "</tr>\n";
    tableHTML += "</thead>\n";
    tableHTML += "<tbody>\n";

    for (var i = 0; i < lines.length; i++) {
      var data = lines[i].split(" ");
      var fileName = data[0];
      var fileSize = data[1];
      if (fileName.length > 0) {
        tableHTML += "<tr>\n";
        tableHTML += "<td>" + fileName + "</td>\n";
        tableHTML += "<td>" + fileSize + "</td>\n";
        tableHTML += "<td>\n";
        tableHTML += "<button class=\"primary\" onclick=\"ws_send_stream('" + fileName + "')\">edit</button>\n";
        tableHTML += "<button class=\"warn\" onclick=\"ws_send_run('" + fileName + "')\">run</button></td>\n";
        tableHTML += "</tr>\n";
      }
    }

    tableHTML += "<tr>\n";
    tableHTML += "<td><input type=\"text\" class=\"smooth\" value=\"/\" id=\"newFile\"/></td>\n";
    tableHTML += "<td>-</td>\n";
    tableHTML += "<td><button class=\"success\" onclick=\"ws_send_create(getNewFileName())\">create</button></td>\n";
    tableHTML += "</tr>\n";
    tableHTML += "</tbody>\n";

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

  ws_send("create \"" + fileName + "\"", log_ws);
  ws_send_ls();
}

function ws_send_write(fileName, content) {
  ws_send_stop(fileName);

  fileName = fixFileName(fileName);
  content = content.replace(/"/g, '\\"');

  ws_send("remove \"/temporary_script\"", log_ws);
  ws_send("create \"/temporary_script\"", log_ws);

  ws_send("stream \"/temporary_script\"", log_ws);

  var ws_send_log = function(msg) {
    status("saving...");
    log_ws(msg);
  };

  var pktsize = 1024;
  for (var i = 0; i < Math.ceil(content.length / pktsize); i++) {
    var begin = i * pktsize;
    var end = begin + pktsize;
    if (end > content.length) end = content.length;

    ws_send(content.substring(begin, end), ws_send_log);
  }
  ws_send("close", log_ws);

  ws_send("remove \"" + fileName + "\"", log_ws);
  ws_send("rename \"/temporary_script\" \"" + fileName + "\"", log_ws);

  ws_send_mem_ls();
  ws_update_status();
}

function ws_send_remove(fileName) {
  ws_send_stop(fileName);

  ws_send("remove \"" + fixFileName(fileName) + "\"", log_ws);

  ws_send_mem_ls();
}

// ===== Startup ===== //
window.addEventListener("load", function() {

  E("scriptsReload").onclick = ws_send_mem_ls;

  E("reconnect").onclick = ws_init;

  E("format").onclick = ws_send_format;

  E("stop").onclick = function() {
    ws_send_stop();
  }

  E("editorReload").onclick = function() {
    ws_send_stream(getEditorFileName());
  };

  E("editorSave").onclick = function() {
    var content = getEditorContent();
    if (!content.endsWith("\n")) content = content + "\n";

    ws_send_write(getEditorFileName(), content);

    E("editorinfo").innerHTML = "saved";
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

  E("editor").onkeyup = function() {
    E("editorinfo").innerHTML = "unsaved changes";
  }

  ws_init();
}, false);