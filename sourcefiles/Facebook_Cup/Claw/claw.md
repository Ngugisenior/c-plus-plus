The Claw
Let's begin by assuming that all targets are fixed at their initial positions. The claw will first move from (0, M) to (X[1], Y[1]), which always requires M-Y[1] vertical movement. It will then move through N-1 "legs", the ith of which takes it from (X[i], Y[i]) to (X[i+1], Y[i+1]). If we let H[i] be the maximum y-coordinate of any target whose x-coordinate is in the interval [X[i], X[i+1]], then the claw must move up to y-coordinate H[i] during the ith leg, making that leg require (H[i]-Y[i])+(H[i]-Y[i+1]) = 2H[i]-Y[i]-Y[i+1] vertical movement. Lastly, the claw will move from (X[N], Y[N]) back to (0, M), which always requires M-Y[N] vertical movement. Summing all of these terms up, the claw's total vertical movement may be conveniently expressed as follows:
2(M + sum{H[1..(N-1)]} - sum{Y[i..N]})
Before proceeding further, we'll need to compute all of the legs' H values. 
There are multiple relatively standard approaches which can get this done in O(N log(N)) time.
For example, we can iterate over the targets and the legs' intervals' right endpoints in non-decreasing order of x-coordinate,
maintain a "convex hull" stack of decreasing target y-coordinates seen so far,
and binary search on it for each leg to find the maximum y-coordinate of any target as far back as the leg's interval's left
endpoint. Or, we can build a segment tree of targets' y-coordinates and then perform a range maximum query on it for each leg.
Now, let’s consider how raising a target affects the answer. 
It clearly increases the magnitude of the negative sum{Y[i..N]} component by 1, 
but it also increases the sum{H[1..(N-1)]} component by some unclear non-negative amount.
We can observe that this amount depends on which other targets are raised, 
meaning that the decision can’t be made for each single target independently. 
However, we can furthermore observe that it only depends on which other targets at the same initial y-coordinate are raised,
meaning that the decisions can be made for each set of targets with a certain shared initial y-coordinate,
independently of all targets at other y-coordinates.
So, let’s consider each possible y-coordinate y in turn. 
Let P be a list of increasing x-coordinates of targets i such that Y[i] = y,
and R be a list of x-coordinate intervals of legs i such that H[i] = y. For each target in P, 
the answer will decrease by 2 if it’s raised. But at the same time, for each interval in R,
This can be restated as an optimization problem in which using a target in P is worth 1 point,
using an interval in R is worth 1 point (as long as no targets within it are also used),
and we want to maximize the total number of points earned.
For convenience, let’s imagine that P contains 2 extra targets at x-coordinates -∞ and ∞, 
and then let DP[i] be the maximum number of points which can be earned using targets
and intervals all ending strictly before P[i]. 
We can have a recurrence of the form 
    DP[i] = max{DP[j] + (number of intervals in R contained strictly between P[j] and P[i]) + 1}, over all 1 ≤ j < i.
However, computing these DP values naively would take O(|P|²) time, where |P| can be as large as N, 
so we’ll need a more efficient approach.
Let’s sort the intervals in R in non-decreasing order of right endpoint,
the answer will increase by 2 if at least one target within that interval is raised.
and then iterate over the targets in P in increasing order.
Before processing each target, we’ll advance through the list of intervals and consider any new ones which end before
that target. Upon considering an interval, the DP values of all targets which occurred strictly before that interval’s left 
endpoint are essentially worth 1 more point than they were before, when it comes to subsequent DP transitions.
Therefore, our approach can be to use a segment tree with lazy propagation to increment a certain prefix of DP values 
for each interval,
and then to query for the maximum of all DP values so far to compute each new DP value. 
This allows these DP computations to all be completed in O(|P| log(|P|)) time, for a total time complexity of O(N log(N))
for the whole solution.
