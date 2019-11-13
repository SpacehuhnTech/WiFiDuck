/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */
// ===== WebSocket Actions ===== //
function load_settings() {
  ws_send("settings", function(msg) {
    var lines = msg.split(/\n/);

    var ssid = lines[0].split("=")[1];
    var password = lines[1].split("=")[1];
    var channel = lines[2].split("=")[1];

    E("ssid").innerHTML = ssid;
    E("password").innerHTML = password;
    E("channel").innerHTML = channel;
  });
}

function ws_connected() {
  load_settings();
}


// ===== Startup ===== //
window.addEventListener("load", function() {

  E("edit_ssid").onclick = function() {
    ws_send("set ssid \"" + prompt("SSID (1-32 chars)", E("ssid").innerHTML) + "\"", function(msg) {
      load_settings();
    });
  };

  E("edit_password").onclick = function() {
    ws_send("set password \"" + prompt("Password (8-64 chars)", E("password").innerHTML) + "\"", function(msg) {
      load_settings();
    });
  };

  E("edit_channel").onclick = function() {
    ws_send("set channel " + prompt("Channel (1-14)", E("channel").innerHTML), function(msg) {
      load_settings();
    });
  };

  ws_init();
}, false);