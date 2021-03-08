# AngryGL

OpenGL clone of the [Unity Angry Bots ECS sample project](https://github.com/UnityTechnologies/AngryBots_ECS)

I also made an [accompnaying video with some discussion](https://www.youtube.com/watch?v=tInaI3pU19Y).

Code is pretty rough but may still prove useful for others doing something similar.

# Building

The project is built using [bazel](https://docs.bazel.build/versions/master/windows.html) for Windows only.

**NOTE:** None of the original assets from Unity's project have been included, you'll need to copy them into the appropriate directories if you want to run the project yourself. The .gitignore file can be a good guide for this.

Project can be built with `bazel build -c opt angrygl:main` and the resulting binary can be found at `bazel-bin/angrygl/main.exe`. The binary assumes that it's being run from the root directory. So build and run would be

```
// From root dir with WORKSPACE in it
bazel build -c opt angrygl:main && ./bazel-bin/angrygl/main.exe
```

# Dependencies

[irrKlang](https://www.ambiera.com/irrklang/index.html) was used as the audio library. I don't believe the license allows for including the library in this repo so audio is disabled. The code which uses the audio is still there but ifdef'd out to show how it was done.

The following libraries were used:

* [stb image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [assimp](https://github.com/assimp/assimp)
* [glad](https://github.com/Dav1dde/glad)
* [ThreadPool](https://github.com/progschj/ThreadPool)
* [glm](https://github.com/g-truc/glm)
* [glfw](https://github.com/glfw/glfw)
