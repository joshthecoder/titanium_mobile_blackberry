// --- Modules ---
//
// The module system provided is based on the CommonJS specification.
// A global function "require" is provided to allow importing
// an external module's exported values. Various types of modules
// are supported such as JavaScript, native extensions, and JSON.

function Module(id, parent) {
  this.id = this.filename = id; // TODO(josh): can we merge id and filename?
  this.parent = parent;
  this.exports = {};
}

exports.Module = Module;

// Global search paths.
// If no relative modules are found for a request
// the system will continue by searching each directory
// listed in the array of search paths.
Module._paths = [ ];

// Modules are only executed once when first required.
// The cache stores the executed module for any future requests.
Module._cache = {};

Module._tryFile = function(path) {
  return $.isFile(path) && path;
}

Module._tryExtensions = function(path) {
  var extensions = Object.keys(Module._extensions);
  for (var i = 0, count = extensions.length; i < count; i++) {
    var filename = Module._tryFile(path + extensions[i]);
    if (filename) {
      return filename;
    }
  }
  return false;
}

Module._tryPath = function(path) {
  var filename;
  var trailingSlash = path.slice(-1) === '/';

  if (!trailingSlash) {
    filename = Module._tryFile(path) || Module._tryExtensions(path);
  }

  if (filename) {
    return filename;
  }
}

Module._resolveFilename = function(request, parent) {
  function throwModuleNotFound() {
    throw new Error('Cannot find module:' + request);
  }

  // If the request is an absolute path, just try it.
  // No other locations will be searched if it fails.
  if (request.charAt(0) === '/') {
    var filename = Module._tryPath(request);
    if (!filename) throwModuleNotFound();
    return filename;
  }

  // The first search path is always relative to the parent.
  var paths = [path.dirname(parent.filename)];

  // If the request does not begin with "../" or "./"
  // (relative only), append the global search paths.
  var requestStart = request.substring(0, 2);
  if (requestStart !== './' || requestStart !== '..') {
    paths.push(Module._paths);
  }

  var filename;

  // Try to find the module file at each search path.
  for (var i = 0, pathCount = paths.length; i < pathCount; i++) {
    filename = Module._tryPath(path.join(paths[i], request));
    if (filename) break; // Stop searching once we find a valid filename.
  }

  // No module found in any of the search paths.
  if (!filename) throwModuleNotFound();

  return filename;
}

Module._extensions = {};

Module._wrapper = [
  '(function(exports, module, require) {',
  '\n});'
];

Module._wrap = function(script) {
  return Module._wrapper[0] + script + Module._wrapper[1];
}

Module._extensions['.js'] = function(module) {
  var script = $.readFile(module.filename);
  if (script.trim().length == 0) {
    return;
  }

  function require(request) {
    return module.require(request);
  }

  // Any variables local to this module only it can access.
  // If you update this list you will also need to update
  // the wrapper function (Module._wrapper).
  var locals = [module.exports, module, require];

  var wrapper = $.runScript(Module._wrap(script), module.filename);
  return wrapper.apply(module.exports, locals);
}

Module._extensions['.json'] = function(module) {
}

Module._extensions['.so'] = function(module) {
  module.exports = $.loadExtension(module.filename);
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

  return loader(this);
}
