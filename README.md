## Raycaster engine

This project aims to create a simple texturized raycaster engine.

**Warning:** This project is still a work in progress.

References:
- [DDA algorithm](https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm))
- [Lodev's article](https://lodev.org/cgtutor/raycasting.html);
- [Transformation Matrix](https://en.wikipedia.org/wiki/Rotation_matrix)
- [Transformation Matrix derivation](https://www.youtube.com/watch?v=EZufiIwwqFA&ab_channel=PenandPaperScience)
- [Cache oblivious matrix transposition](https://en.algorithmica.org/hpc/external-memory/oblivious/)
- [Cache oblivious algorithms](https://erikdemaine.org/papers/BRICS2002/paper.pdf)
- [Wolfenstein 3d original source code](https://github.com/id-Software/wolf3d)

## Quick start

Dependencies:
- [raylib](https://www.raylib.com/) and all its own dependencies.

```sh
$ ./build.sh
$ ./bin/raycaster
```

## To-do list
- [ ] Find a way to invert the frame buffer matrix, so that we process lines horizontally
