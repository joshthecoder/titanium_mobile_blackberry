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

