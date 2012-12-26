// Setup the root module and the global require.
var Module = module.Module;
Module._root = new Module('.');
require = function(request) { return Module._root.require(request); }
