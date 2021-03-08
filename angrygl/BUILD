cc_library(
    name = "aabb",
    hdrs = ["aabb.h"],
    deps = [
        "@glm",
    ],
)

cc_library(
    name = "spritesheet",
    hdrs = ["spritesheet.h"],
    deps = [
        "@glm",
    ],
)

cc_library(
    name = "model",
    hdrs= ["model.h"],
    srcs = ["model.cc"],
    deps = [
        ":player_mesh",
        "//opengl:shader",
        "//opengl:texture",
        "//opengl:vertex",
        "//:assimp",
        "//:assimp_include",
        "//stb:image",
    ],
)

cc_library(
    name = "enemy_spawner",
    hdrs = ["enemy_spawner.h"],
    srcs = ["enemy_spawner.cc"],
    deps = [
        ":enemy",
    ]
)

cc_library(
    name = "geom",
    hdrs = ["geom.h"],
    deps = [
        "@glm",
    ],
)

cc_library(
    name = "capsule",
    hdrs = ["capsule.h"],
)

cc_library(
    name = "enemy",
    hdrs = ["enemy.h"],
    deps = [
        ":capsule",
        "@glm",
    ],
)

cc_library(
    name = "bullet_store",
    hdrs = ["bullet_store.h"],
    srcs = ["bullet_store.cc"],
    deps = [
        ":aabb",
        ":enemy",
        ":capsule",
        ":spritesheet",
        ":geom",
        "@glm",
        "//glad",
        "//lib:threadpool",
    ],
)

cc_library(
    name = "player_mesh",
    srcs = ["player_mesh.cc"],
    hdrs = ["player_mesh.h"],
    deps = [
        "//opengl:shader",
        "//opengl:texture",
        "//opengl:vertex",
    ],
)

cc_library(
    name = "player_model",
    srcs = ["player_model.cc"],
    hdrs = ["player_model.h"],
    deps = [
        ":player_mesh",
        "//:assimp",
        "//:assimp_include",
        "//opengl:shader",
        "//opengl:texture",
        "//opengl:vertex",
        "//stb:image",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    data = glob([
        "*.vert",
        "*.frag",
    ]) + glob(["assets/**/*"]),
    defines = [
        "GLFW_INCLUDE_NONE",
    ],
    copts = [
        "/O2",
    ],
    linkopts = [
        "opengl32.lib",
    ],
    deps = [
        ":player_model",
        ":spritesheet",
        ":enemy",
        ":enemy_spawner",
        ":bullet_store",
        ":geom",
        "//glad",
        ":model",
        "//stb:image",
        "//lib:threadpool",
        #"//irrklang:irrklang",
        "@glfw//:include",
        "@glfw//:src",
        "@glm",
    ],
)
