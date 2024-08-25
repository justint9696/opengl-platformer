# Level Editor Design
## States
* Add
* Remove
* Edit
* Highlight
* Idle

## Features
* Place entities anywhere in the level
* Highlight multiple blocks and be able to edit or remove all of them at once
* Move the camera around the level
* 

# States
* Holding shift enters select mode
* Pressing A enters place mode, ESC to exit, Left Click to Place
    * In this mode there is should be an indicator of where the entity will be placed
* Left click on a block when not in Place mode moves it
    * If the block is part of a highlighted segment, move all highlighted blocks
* Right click removes block in any state
