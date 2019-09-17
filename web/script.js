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

    ws_connected();
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

  setInterval(ws_msg_queue_update, 1);
}

function ws_send(message, callback) {
  ws_msg_queue.push({
    "message": message,
    "callback": callback
  });
}