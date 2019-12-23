package(default_visibility = ["//visibility:public"])

cc_library(
    name = "irrklang",
    hdrs = glob(["include/*.h"]),
    #strip_include_prefix = "include/",
    deps = [
        ":import",
    ],
)

cc_import(
    name = "import",
    interface_library = "lib/Winx64-visualStudio/irrKlang.lib",
    shared_library = "bin/winx64-visualStudio/irrKlang.dll",
)
