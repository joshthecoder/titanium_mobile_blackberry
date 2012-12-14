// Setup the root module and the global require.
var Module = module.Module;
Module._root = new Module('.');
require = function(request) { return Module._root.require(request); }

var foo = require('foo');
$.print('foo: ' + JSON.stringify(foo));
$.print('foo.y() -> ' + foo.y());
