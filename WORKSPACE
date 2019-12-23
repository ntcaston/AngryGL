load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# GLFW
http_archive(
  name = "glfw",
  urls = ["https://github.com/glfw/glfw/releases/download/3.3/glfw-3.3.zip"],
  sha256 = "36fda4cb173e3eb2928c976b0e9b5014e2e5d12b9b787efa0aa29ffc41c37c4a",
  strip_prefix = "glfw-3.3",
  build_file = "@//:glfw.BUILD",
)

# GLM
http_archive(
  name = "glm",
  urls = ["https://github.com/g-truc/glm/releases/download/0.9.9.6/glm-0.9.9.6.zip"],
  sha256 = "9db7339c3b8766184419cfe7942d668fecabe9013ccfec8136b39e11718817d0",
  strip_prefix = "glm",
  build_file = "@//:glm.BUILD",
)

# ASSIMP
#http_archive(
#  name = "assimp",
#  urls = ["https://github.com/assimp/assimp/archive/v4.1.0.zip"],
#  sha256 = "407be74f44f488fcf1aac3492d962452ddde89561906e917a208c75e1192bcdc",
#  strip_prefix = "assimp-4.1.0",
#  build_file = "@//:assimp.BUILD",
#)
