#ifndef TI_RUNTIME_OPTIONS_H
#define TI_RUNTIME_OPTIONS_H

namespace titanium {

// A quick and dirty flag parser.
class Flags {
 public:
  // Set flags from the command line arguments.
  static void setFromCommandLine(int argc, char* argv[]);

  // Reset all flags to their default values.
  static void resetAll();

  // Returns currently set flags.
  static const Flags& current() {
    return flags;
  }

  const char* processName() const {
    return processName_;
  }
  const char* v8Flags() const {
    return v8Flags_;
  }
  const char* mainScriptPath() const {
    return mainScriptPath_;
  }

 private:
  Flags()
    : processName_("app")
    , v8Flags_(0)
    , mainScriptPath_("main.js") { }

  static Flags flags;

  const char* processName_;
  const char* v8Flags_;
  const char* mainScriptPath_;

};

} // namespace titanium

#endif
