// Zygote.
//
// The zygote is a function which is responsible for
// bootstraping a new JavaScript execution environment.
// It should be called within a new context before
// any application scripts are run.
(function($) {
  function debug(msg) {
    // TODO(josh): enable debug via commandline flag.
    true && $.print(msg);
  }


  // --- Modules ---
  //
  // The module system provided is based on the CommonJS specification.
  // A global function "require" is provided to allow importing
  // an external module's exported values. Various types of modules
  // are supported such as JavaScript, native extensions, and JSON.

  function Module(id, parent) {
    this.id = id;
    this.parent = parent;
  }

  // If no relative modules are found for a request
  // the system will continue by searching each directory
  // listed in the array of search paths.
  Module._paths = [ ];

  Module._resolveFilename = function(request, parent) {
  }

  Module.prototype.require = function(req) {
    debug('require(' + req + ')');
  }

  // Setup the root module and the global require.
  Module._root = new Module('.');
  require = function(request) { return Module._root.require(request); }


  // --- test ---

  require('foo');
});
