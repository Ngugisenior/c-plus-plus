City Lights
Let’s build a binary tree in which each node corresponds to a possible building rectangle worth considering. 
This tree will have S internal nodes, each of which is associated with a star, with its corresponding rectangle reaching up 
to just below that star and extending as far left and right as possible without hitting any other stars. 
     The tree will also have S+1 leaves, each of which is associated with an x-coordinate interval 
 (either one of the S-1 x-coordinate intervals between stars, or the infinite x-coordinate interval on the far left, or
the one on the far right), with its corresponding rectangle exclusively spanning that interval and reaching infinitely high up.
The tree’s root node will be associated with the lowest star (breaking ties arbitrarily), and then each internal node will have
two children — the left child will be associated with the next-lowest star to the left of this star whose x-coordinate is within
this node’s corresponding rectangle (or will be a leaf node if there are no such stars), and the right child will be defined 
similarly. This tree may be constructed in O(S log(S)) time by iterating through the stars in non-decreasing order of height
while maintaining an ordered set of x-coordinate intervals of nodes’ corresponding rectangles.
Each window is covered by one or more of the 2S+1 nodes’ rectangles. We can make the critical observation that these valid nodes
for any given window form a contiguous path in the tree, including either a single node x, or running up from some node x to
one of x’s ancestors. For example, the path for a window above and to the left of all of the stars would consist 
only of the leftmost leaf node (which corresponds to a rectangle to the left of the leftmost star 
which extends infinitely far left and reaches infinitely high up). Meanwhile, the path for a window below and 
to the left of all of the stars would start at the leftmost leaf node and go all the way up to the tree’s root node
(which corresponds to a rectangle which reaches up to just below the lowest star and extends infinitely far left and right).
Computing these paths for all of the windows may be done in O(W log(S)) time by re-using the set of x-coordinate intervals from before.
Assuming for the moment that all windows are visible, we’re interested in selecting a minimum-size set of nodes in the tree 
such that every window’s path includes at least one selected node. This can be done greedily in O(S+W) time by recursing 
through the tree, keeping track of the maximum depth m of the top node on any not-yet-satisfied path which began in the current subtree,
and selecting the current node to satisfy all such paths when necessary (specifically, when m is equal to the current node’s depth).
Adding “probability” into the mix is still manageable with this formulation, though it introduces two unknowns into the process 
which must be handled with dynamic programming. Let DP[i, s, m] be the number of subsets of visible windows
whose paths begin in node i’s subtree, which result in a certain m value (as described above) 
coming out of i’s subtree after s nodes have been selected in i’s subtree.
For each node, merging its (at most) two children’s DP values together may be done in O(S²W²) time naively, or in O(SW²) 
time with the help of some precomputed DP prefix sums. Then, if P is the list of window paths which begin at the current node,
we can sort the depths of their top nodes and incorporate the effects of all of their possible subsets on all of the current 
node’s states’ m values in O(|P| log(|P|) + |P|SW) time. Finally, we should update all of the current node’s states to factor 
in greedily selecting the current node when necessary in another O(SW) time. 
This brings us to a total time complexity of O(SW²) per node, or O(S²W²) for the entire algorithm.
All that remains from there is computing the final answer based on the root node’s DP values.
