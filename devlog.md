# Developer Log
As this project has grown in scale, the number of problems I have encountered has increased. Here are some of the problems I have come across, my initial thoughts on how to solve them, and their solutions. In addition to acting as a time capsule for my thoughts, the solutions & approaches will be documented for future use.

## 2024/08/04 - 11:25am
### Problem
There are many problems with the chunking system. With my approach, I am trying to figure it out as I go along, but continuity issues are arising. The problem right now is swapping chunks in a proper order. When chunks are swapped, their indices and entity array pointers are swapped accordingly. The problem that arises is how the aabb's for pages are calculated and how the chunk associated with an entity's position is calculated. If the chunk index and position are modified before the chunks are swapped, the entities associated with those indices cannot be removed. If the chunk index and position are swapped after the pages are swapped, the page aabb's are calculated incorrectly.

### Solution
As previously mentioned, part of the solution was addressing the continuity issues when calculating aabb's. This was resolved by calculating the aabb outside of the load data function and before requesting the chunk index. Additionally, rather than clearing the arrays, it made more sense to utilize the free list and free the block of memory used for the array. Lastly, the other pages' array pointers and aabbs can be swapped, rather than just changing the indices as before.


## 2024/08/31 - 03:06pm
### Problem
I am at the point where I need to figure out how the level editor exports and imports files efficiently. One of the first problems that is coming up is determining the position of the starting chunk, and even more so, what determines the starting chunk? Additionally, what happens when I want to add a new page to the level? I have thought about this before, and my solution was to use an insert function within dynamic arrays. If I do this (and I probably will), then I will need to modify the array header or place heavy restrictions on inserting (inserting into a dynamic array can only be between the starting and ending index of the current length). This will allow new pages to be inserted into the offset array without having to rewrite the entire file. This could produce complications with storage later down the line, but for now I think it's okay. The file is only 1.3KB right now. 

### Solution
TBA
