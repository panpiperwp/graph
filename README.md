# Dynamic sparse graph.

Implementation of a dynamic sparse graph.

It turns out that the graph is a bit of an awkward data structure to work with. On one hand, one might like the freedom to alter vertex and edge data. On the other, it seems necessary for an encapsulating class to manage vertex and edge additions/removals. Therefore the Edge and Vertex objects remain structs, but I advise against directly manipulating the containers they contain -- use the Graph class for that. The Graph class will ensure that edge and vertex pointers are consistent among vertices and edges, respectively, throughout addition and removal.

In any case, I seemed to have damned myself in my desire for a dynamic graph. One can add/remove vertices/edges but as a result the vertices cannot be stored in contiguous memory. Actually, it is possible, but then it becomes challenge to for the programmer to keep track of the "location" of the vertices. By "location", I mean "identifying feature". In a vector, this concept would be represented as the index of the vector, e.g. vertex 1 is at vertices[0], vertex 2 is at vertices[1], etc. Removing a vertex changes the index and by extension the location of the other vertices. The next best thing is a hash map, so I employ an unordered map to be the basis of the Graph class. (Word is that it's not that great though: https://www.youtube.com/watch?v=Hi0dZcuucvM)

Now for some notes you should read:
- The file (Graph.h) makes use of shared pointers. I do this not because of any sort of personal ineptness regarding pointers, but because the general attitude online is that pointers are the root of all evil. Hopefully this adjustment will make the code easier for you to work with.
- None of the methods check for existence unless it can be done whilst they perform their main duty. That is, if you try to remove a nonexistent edge or vertex, expect problems.
- Following the above note, I have added asserts (from <cassert>) when possible. This should clarify where errors are occurring if you attempt to do the impossible.
- You can reserve memory for the graph! See the Doxygen documentation and take a look at the constructors for the Graph class.

I hope that you find this class useful!

Update 6/18/2015:
- Clarified the type of graph; it turns out that a "flexible graph" is actually a mathematical term and not what I have set out to create. Dynamic (can add and remove vertices and edges) and sparse (implements an adjacency list so keep the number edges O(n)) are much more descriptive terms
- In an attempt to mimic the STL, I renamed most of the objects. Commenting is better, too, and documentation is complete.
- The big 5 have been implemented! (It has to be said that the move semantics may be lacking, however.)
