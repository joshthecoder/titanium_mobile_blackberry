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

  // --- Path ---
  //
  // Some path handling utilities used internally.
  var path = {
    // Extract the file extension from a path.
    //     path('foo.js') -> '.js'
    extname: function(path) {
      var index = path.lastIndexOf('.');
      return (index && path.substr(index)) || '';
    }
  }


  // --- Modules ---
  //
  // The module system provided is based on the CommonJS specification.
  // A global function "require" is provided to allow importing
  // an external module's exported values. Various types of modules
  // are supported such as JavaScript, native extensions, and JSON.

  function Module(id, parent) {
    this.id = this.filename = id; // TODO(josh): can we merge id and filename?
    this.parent = parent;
  }

  // If no relative modules are found for a request
  // the system will continue by searching each directory
  // listed in the array of search paths.
  Module._paths = [ ];

  // Modules are only executed once when first required.
  // The cache stores the executed module for any future requests.
  Module._cache = { };

  Module._resolveFilename = function(request, parent) {
    return 'foo.so';
  }

  Module._extensions = { };

  Module._extensions['.js'] = function(filename) {
    debug('Load JavaScript module: ' + filename);
  }

  Module._extensions['.json'] = function(filename) {
    debug('Load JSON module: ' + filename);
  }

  Module._extensions['.so'] = function(filename) {
    debug('Load native extension module: ' + filename);
  }

  Module.prototype.require = function(request) {
    debug('require(' + request + ')');

    var filename = Module._resolveFilename(request, this);

    var module = Module._cache[filename];
    if (module) {
      return module.exports;
    }

    module = Module._cache[filename] = new Module(filename, this);

    var loadFailed = true;
    try {
      module.load();
      loadFailed = false;
    } finally {
      if (loadFailed) {
        delete Module._cache[filename];
      }
    }

    return module.exports;
  }

  Module.prototype.load = function() {
    var ext = path.extname(this.filename) || '.js';

    var loader = Module._extensions[ext];
    if (!loader) throw 'Unknown module extension: ' + ext;

    return loader(this.filename);
  }

  // Setup the root module and the global require.
  Module._root = new Module('.');
  require = function(request) { return Module._root.require(request); }


  // --- test ---

  var foo = require('foo');
  debug('foo: ' + JSON.stringify(foo));
});
