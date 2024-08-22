/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

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
  if (fileName.length > 0) {
    if (fileName[0] != '/') {
      fileName = '/' + fileName;
    }

    fileName = fileName.replace(/ /g, '\-');
  }
  return fileName;
}

// ===== DOM Manipulation ===== //
function status(mode) {
  current_status = mode;

  if (mode == "connected") {
    E("status").style.backgroundColor = "#3c5";
  } else if (mode == "disconnected") {
    E("status").style.backgroundColor = "#d33";
  } else if (mode.includes("problem") || mode.includes("error")) {
    E("status").style.backgroundColor = "#ffc107";
  } else /*if (mode == "connecting...")*/ {
    E("status").style.backgroundColor = "#0ae";
  }

  E("status").innerHTML = mode;
}

// ===== Web Socket ===== //
function log_ws(msg) {
  log("[WS] " + msg);
}

function set_version(str) {
  E("version").innerHTML = str;
}

var ws = null; // web socket instance
var ws_callback = log_ws; // message receive callback
var ws_msg_queue = []; // queue for outgoing messages
var cts = false; // clear to send flag for message queue

var current_status = "";

var ws_queue_interval = null;

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

function ws_send(message, callback, force = false) {
  if (!message.endsWith('\n')) message += '\n';

  ws_send_raw(message, callback, force);
}

function ws_send_raw(message, callback, force = false) {
  var obj = {
    "message": message,
    "callback": callback
  };

  if (force) {
    ws_msg_queue.unshift(obj);
  } else {
    ws_msg_queue.push(obj);
  }
}

function ws_update_status() {
  ws_send("status", status);
}

function ws_init() {
  status("connecting...");

  ws = new WebSocket("ws://" + location.host + "/ws");
  ws.onopen = function(event) {
    log_ws("connected");
    status("connected");

    ws_send("close", log_ws, true);
    ws_send("version", set_version);

    ws_connected();
  };

  ws.onclose = function(event) {
    log_ws("disconnected");
    status("disconnected");
  };

  ws.onmessage = function(event) {
    var msg = event.data;

    log_ws(msg);

    if (ws_callback && msg.length > 0) {
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

  if (ws_queue_interval) clearInterval(ws_queue_interval);
  ws_queue_interval = setInterval(ws_msg_queue_update, 1);
}