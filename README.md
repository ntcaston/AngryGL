# AngryGL

OpenGL clone of the [Unity Angry Bots ECS sample project](https://github.com/UnityTechnologies/AngryBots_ECS)

I also made an [accompnaying video with some discussion](https://www.youtube.com/watch?v=tInaI3pU19Y).

Code is pretty rough but may still prove useful for others doing something similar.

# Building

The project is built using [bazel](https://docs.bazel.build/versions/master/windows.html) for Windows only.

**NOTE:** None of the original assets from Unity's project have been included, you'll need to copy them into the appropriate directories if you want to run the project yourself.

Project can be built with `bazel build -c opt angrygl:main` and the resulting binary can be found at `bazel-bin/angrygl/main.exe`
