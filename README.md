# Knight Libs

Collection for small gamdev libraries for C++ + SFML.

## Libraries

- `loader` - load resources (textures, music, sounds, fonts)

  depends on SFML
- `animation` - create animations

  depends on SFML, `loader`
- `game` - container for necessary game objects

  depends on SFML, `loader`
- `gui` - basic GUI (button, grid)

  depends on SFML, `animation`, `simple_ecs`
- `hex` - a `Hex` type allowing calculations needed for hex grid implementation
- `hex_map` - draws hex grid map and handles the camera

  depends on `simple_ecs`, `animation`, `hex`, `loader`
- `pathfinding` - Dijkstra's shortest path implementation

  depends on `hex`
- `simple_ecs` - entity component system implementation

  more information: https://github.com/lchsk/simple-ecs
  
## Examples

See `examples/` directory.

- `animation` - simple animation
- `map` - drawing a hex grid map, using a camera, a GUI button
