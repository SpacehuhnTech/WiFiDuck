/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */


// ========== Global Variables ========== //

// ! List of files returned by "ls" command
var file_list = "";

// ! Variable to save interval for updating status continously
var status_interval = undefined;

// ! Unsaved content in the editor
var unsaved_changed = false;

// ! Flag if editor has loaded a file yet
var file_opened = false;

// ========== Global Functions ========== //

// ===== Value Getters ===== //
function get_new_filename() {
  return E("newFile").value;
}

function get_editor_filename() {
  return E("editorFile").value;
}

function set_editor_filename(filename) {
  return E("editorFile").value = filename;
}

function get_editor_content() {
  var content = E("editor").value;

  if (!content.endsWith("\n"))
    content = content + "\n";

  return content;
}

// ! Update status until it's no longer "running"
function check_status() {
  if (current_status.includes("running") || current_status.includes("saving"))
    ws_update_status();
  else
    stop_status_interval();
}

// ! Start interval that checks and updates the status continously
function start_status_interval() {
  if (status_interval) return; // !< Only continue if status_interval not set

  ws_update_status(); // !< Get current status
  status_interval = setInterval(check_status, 500); // !< Start interval
}

// ! Stop interval that checks and updates the status continously
function stop_status_interval() {
  if (!status_interval) return; // !< Only continue if status_interval was set

  // ! Stop interval and unset variable
  clearInterval(status_interval);
  status_interval = undefined;
}

// ! Append string to script content
function append(str) {
  E("editor").value += str;
}

// ! Updates file list and memory usage
function update_file_list() {
  ws_send("mem", function(msg) {
    var lines = msg.split(/\n/);
    
    if(lines.length == 1) {
      console.error("Malformed response:");
      console.error(msg);
      return;
    }

    var byte = lines[0].split(" ")[0];
    var used = lines[1].split(" ")[0];
    var free = lines[2].split(" ")[0];

    var percent = Math.floor(byte / 100);
    var freepercent = Math.floor(free / percent);

    E("freeMemory").innerHTML = used + " byte used (" + freepercent + "% free)";

    file_list = "";

    ws_send("ls", function(csv) {
      file_list += csv;

      var lines = file_list.split(/\n/);
      var tableHTML = "<thead>\n";

      tableHTML += "<tr>\n";
      tableHTML += "<th>File</th>\n";
      tableHTML += "<th>Byte</th>\n";
      tableHTML += "<th>Actions</th>\n";
      tableHTML += "</tr>\n";
      tableHTML += "</thead>\n";
      tableHTML += "<tbody>\n";

      for (var i = 0; i < lines.length; i++) {
        var data = lines[i].split(" ");
        var fileName = data[0];
        var fileSize = data[1];

        if (fileName.length > 0) {
          if (i == 0 && !file_opened) {
            read(fileName);
          }
          tableHTML += "<tr>\n";
          tableHTML += "<td>" + fileName + "</td>\n";
          tableHTML += "<td>" + fileSize + "</td>\n";
          tableHTML += "<td>\n";
          tableHTML += "<button class=\"primary\" onclick=\"read('" + fileName + "')\">edit</button>\n";
          tableHTML += "<button class=\"warn\" onclick=\"run('" + fileName + "')\">run</button>\n";
          tableHTML += "</tr>\n";
        }
      }
      tableHTML += "</tbody>\n";

      E("scriptTable").innerHTML = tableHTML;
    });
  });
}

// ! Format SPIFFS
function format() {
  if (confirm("Format SPIFFS? This will delete all scripts!")) {
    ws_send("format", log_ws);
    alert("Formatting will take a minute.\nYou have to reconnect afterwards.");
  }
}

// ! Run script
function run(fileName) {
  ws_send("run \"" + fixFileName(fileName) + "\"", log_ws);
  start_status_interval();
}

// ! Stop running specific script
function stop(fileName) {
  ws_send("stop \"" + fixFileName(fileName) + "\"", log_ws, true);
}

// ! Stop running all scripts
function stopAll() {
  ws_send("stop", log_ws, true);
}

// ! Recursive read from stream
function read_stream() {
  ws_send("read", function(content) {
    if (content != "> END") {
      E("editor").value += content;
      read_stream();
      status("reading...");
    } else {
      ws_send("close", log_ws);
      ws_update_status();
    }
  });
}

// ! Open stream to a file
function read(fileName) {
  stop(fileName);

  fileName = fixFileName(fileName);

  set_editor_filename(fileName);
  E("editor").value = "";

  ws_send("stream \"" + fileName + "\"", log_ws);

  read_stream(); // !< Read file contents (recursively)

  file_opened = true;
}

// ! Create a new file
function create(fileName) {
  stop(fileName);

  fileName = fixFileName(fileName);

  if (file_list.includes(fileName + " ")) {
    read(fileName);
  } else {
    set_editor_filename(fileName);
    E("editor").value = "";

    ws_send("create \"" + fileName + "\"", log_ws);
    update_file_list();
  }
}

// ! Delete a file
function remove(fileName) {
  stop(fileName);
  ws_send("remove \"" + fixFileName(fileName) + "\"", log_ws);
  update_file_list();
  unsaved_changed = true;
}

function autorun(fileName) {
  ws_send("set autorun \"" + fixFileName(fileName) + "\"", log_ws);
}

// ! Write content to file
function write(fileName, content) {
  stop(fileName);

  fileName = fixFileName(fileName);

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

    ws_send_raw(content.substring(begin, end), ws_send_log);
  }

  ws_send("close", log_ws);

  ws_send("remove \"" + fileName + "\"", log_ws);
  ws_send("rename \"/temporary_script\" \"" + fileName + "\"", log_ws);

  ws_update_status();
}

// ! Save file that is currently open in the editor
function save() {
  write(get_editor_filename(), get_editor_content());
  unsaved_changed = false;
  E("editorinfo").innerHTML = "saved";
  update_file_list();
}

// ! Function that is called once the websocket connection was established
function ws_connected() {
  update_file_list();
}

// ========== Startup ========== //
window.addEventListener("load", function() {
  E("reconnect").onclick = ws_init;
  E("scriptsReload").onclick = update_file_list;
  E("format").onclick = format;
  E("stop").onclick = stopAll;

  E("editorReload").onclick = function() {
    read(get_editor_filename());
  };

  E("editorSave").onclick = save;

  E("editorDelete").onclick = function() {
    if (confirm("Delete " + get_editor_filename() + "?")) {
      remove(get_editor_filename());
    }
  };

  E("editorDownload").onclick = function() {
    download_txt(get_editor_filename(), get_editor_content());
  };

  E("editorStop").onclick = function() {
    stop(get_editor_filename());
  }

  E("editorRun").onclick = function() {
    if (unsaved_changed) {
      save();
    }

    run(get_editor_filename());
  };

  E("editor").onkeyup = function() {
    unsaved_changed = true;
    E("editorinfo").innerHTML = "unsaved changes";
  }

  E("editorAutorun").onclick = function() {
    if (confirm("Run this script automatically on startup?\nYou can disable it in the settings."))
      autorun(get_editor_filename());
  }

  // ! Make all <code>s append to the editor when clicked
  var codes = document.querySelectorAll("code");
  for (var i = 0; i < codes.length; i++) {
    codes[i].addEventListener("click", function() {
      append(this.innerHTML + " \n");
    });
  }

  ws_init();
}, false);