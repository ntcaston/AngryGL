package(default_visibility = ["//visibility:public"])

cc_library(
    name = "include",
    hdrs = glob(["include/GLFW/*.h"]),
    # strip_include_prefix = "include/",
)

cc_library(
    name = "src",
    hdrs = [
        # Common
        "src/internal.h",
        "src/mappings.h",

        # _GLFW_WIN32
        # TODO(nick.c): Grab appropriate deps from CMakeLists
        "src/win32_platform.h",
        "src/win32_joystick.h",
        "src/wgl_context.h",
        "src/egl_context.h",
        "src/osmesa_context.h",
    ],
    srcs = [
        # Common
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c",

        # _GLFW_WIN32
        "src/win32_init.c",
        "src/win32_joystick.c",
        "src/win32_monitor.c",
        "src/win32_time.c",
        "src/win32_thread.c",
        "src/win32_window.c",
        "src/wgl_context.c",
        "src/egl_context.c",
        "src/osmesa_context.c",
    ],
    defines = [
        "_GLFW_WIN32",
    ],
    deps = [
        ":include",
    ],
    linkopts = [
        "user32.lib",
        "gdi32.lib",
        "shell32.lib",
    ],
)

