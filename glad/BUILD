package(default_visibility = ["//visibility:public"])

cc_library(
    name = "glad",
    hdrs = glob(["include/**/*.h"]),
    srcs = glob(["src/**/*.c"]),
    strip_include_prefix = "include/",
    linkopts = [
        "opengl32.lib",
    ],
)
