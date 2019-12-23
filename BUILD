package(default_visibility = ["//visibility:public"])

cc_import(
    name = "assimp",
    interface_library = "assimp/lib/assimp-vc140-mt.lib",
    shared_library = "assimp/bin/assimp-vc140-mt.dll",
)

ASSIMP_INCLUDE_HDR_GLOBS = [
    "assimp/include/**/*.hpp",
    "assimp/include/**/*.h",
    "assimp/include/**/*.inl",
]

cc_library(
    name = "assimp_include",
    hdrs = glob(ASSIMP_INCLUDE_HDR_GLOBS),
    strip_include_prefix = "assimp/include/",
)
