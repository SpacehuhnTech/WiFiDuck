const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const vm = require("node:vm");

const version = { textContent: "" };
const listeners = [];
const context = {
  console,
  document: {
    cookie: "unrelated=value; CURRENT_VERSION=1.3.0.0; another=value",
    getElementById: id => id === "version" ? version : null
  },
  window: { addEventListener: (event, callback) => listeners.push(callback) }
};
vm.createContext(context);
vm.runInContext(fs.readFileSync(path.join(__dirname, "../web/script.js"), "utf8"), context);
listeners.forEach(callback => callback());
assert.equal(version.textContent, "Version 1.3.0.0");
context.set_version("Version 1.3.0.0 (5)");
assert.equal(version.textContent, "Version 1.3.0.0 (5)");
context.document.getElementById = () => null;
assert.doesNotThrow(() => context.set_version("1.3.0.0"));
context.document.cookie = "CURRENT_VERSION_SUFFIX=wrong";
assert.equal(context.getCookie("CURRENT_VERSION"), "");
console.log("Cookie fallback and WebSocket version display tests passed");
