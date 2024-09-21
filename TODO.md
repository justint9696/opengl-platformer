## OpenGL Platformer TODO
- [x] Address grid updating with camera movement
    - [x] Test it out on a flat level without chunks first
- [x] Level chunking
    - [x] When swapping page indexes, the chunk is misaligned; swap array
      pointer instead
- [x] General code documentation for prototypes
- [ ] Level editor initial implementations
    - [x] Load level data into temp file
    - [x] Store entities within temp file when swapping pages
    - [x] Export data from temp file into a level file
    - [x] Check if mouse is hovering an entity
    - [x] Manipulate the hovered entity with the mouse
    - [x] Figure out a system to place blocks
    - [ ] Figure out a system to change textures
    - [x] Right click to remove blocks
    - [x] Block snapping
    - [x] Draw grid for block snapping
    - [x] Make swap chunks based on camera position rather than the player
      position
    - [ ] Add above and below pages && allow them to be swapped
    - [ ] Grab entities under selection rectangle and push to dynamic array
    - [ ] Make edit state work for all entities within the dynamic array
    - [x] Calculate the correct chunk position when importing the level
- [ ] UI Debug Methods
    - [ ] Be able to draw a transparent quad anywhere on the screen
    - [ ] Cleanup the ui file and make it more presentable
    - [ ] Add brief documentation for prototypes
