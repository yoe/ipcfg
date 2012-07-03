This is ipcfg 0.6, the second rewrite of ipcfg. ipcfg 0.4 was never
released; the first rewrite, 0.5, wasn't either. This is 0.6, the second
rewrite, and it is done in [D](http://dlang.org).

# Compiling

D has seen a fairly significant change recently; the original language
is now referred to as "D1", whereas the new version is "D2". The old
dialect will not be supported after December of this year anymore by D
upstream, so I didn't think it appropriate to write ipcfg in D1 anymore.

I'm currently using the proprietary 'dmd' compiler, which was written
and is maintained by Walter Bright, inventor of the D language. However,
until and unless the code compiles with gdc (a fully free (as in speech)
compiler which uses the same frontend as dmd but the gcc backend), it
will not be uploaded to Debian. Since it's too late for ipcfg to be
part of wheezy now anyway, and since I expect that gdc will be updated
to (at least) gdc 4.7 before wheezy+1 is released, I'm not giving this
high priority at the moment.

With that out of the way,

compiling is done like this:

make DC=dmd

if you have dmd installed. I'm specifically not changing the gdmd
default, to remind me of the above. Currently, however, gdmd will fail.

You can download gdmd from [http://dlang.org/download.html]

# History

The original version of ipcfg (ipcfg 0.4, in the 04 branch) used a sort
of linked list turing machine of function pointers. If that sounds like
a bad idea, that's probably because it is. Additionally, ipcfg 0.4 only
knew about bringing an interface up, not down.

To fix that, I realized that I had to change a significant body of the
code, and would have to throw away much of what I'd already written. I
started a first rewrite, ipcfg 0.5, but the fundamental problem (the
fact that it still was a linked list turing machine of function
pointers) remained. It was very hard for any extension to ipcfg to
figure out where to insert its code, since there was no formal
representation of the dependency structure.

So I basically gave up.

Then, some time later, I started using [puppet](http://puppetlabs.com).
On a very high level, this tool verifies the system state, compares that
against the wanted state, and figures out what it needs to do to bring
the first in sync with the second.

This is precisely what I wanted ipcfg to do, so I did some
investigation. I briefly considered using haskell to implement a new
version of ipcfg, but quickly decided against it; I wanted to write an
application that reacts to changes in the environment; while possible in
haskell, this requires you use some fairly advanced features of the
language -- not the best way to learn. Additionally, I would have to
write glue code to libnl in order to be able to use it. If having to use
advanced parts of a language is a bad idea, then writing glue code for a
language you don't know yet is even more so.

I then found out about the Boost Graph Library, which sounded somewhat
interesting. But C++ isn't my favourite language for various reasons, so
I decided against that, too.

For unrelated reasons around the time when I was investigating BGL, I
started browsing and learning about D. While doing so, I realized that
this was fairly close to what I needed...

# Organization

(This section will be updated as code evolves...)

## Theory

The system creates a [graph](http://en.wikipedia.org/wiki/Graph_theory)
in which the nodes are system states, and the edges are transitions. For
instance, there could be a node describing "eth0 has a link", and a node
"eth0 has an IP address". Given these two nodes, there could be an edge
going from the node "eth0 has a link" to "eth0 has an IP address". When
traversing that edge, the edge would apply the IP address to the
interface.

When trying to figure out what to do to update the system configuration,
ipcfg would first build (the relevant part of) the graph representing
the known configurations; then it would search the graph for the node
describing the current system state; next, it would mark one or more
nodes in the graph that represent "wanted" states; and then it would
find the shortest path between the "current" node and any of the wanted
states. It would ignore those wanted states that are unreachable, and
try to walk the path which brings us to the wanted state with the
highest priority.

## Details

The decision system as described above is implemented in edge.d, node.d,
and path.d. The plan is that future versions of ipcfg (when D supports
shared objects properly) will allow extending the edge and node
interfaces; what's in path.d is really internal cookery.

### Edge

Edges have a function `estimate` which returns an integer. The value of
this integer can be 0, which means "no change is required; we have
already passed this edge"; or it can be 1000, meaning "traversing this
edge will be absolutely positively impossible"; or it can be anything in
between, with 500 meaning "we have no clue one way or another whether
this will work, but we'll try", 1 meaning something like "it's extremely
likely this will be possible, but it could still fail" and 999 meaning
"it's highly unlikely, but you could be lucky".

Edges have a `cost` property, which is meant to influence the return
value of the `estimate` function. The scale of this `cost` property is
the same as the scale of the `estimate` function, and the default Edge
implementation returns whatever the value of cost is, but this isn't
required. The point of the `cost` property is to allow an advanced user
to specify it in a configuration file, and thereby influence the
decision making system.

The `estimate` function must not change the system state at any cost.
The `estimate` function must also not return a different value _unless_
something in the system state has changed, or the cost property has been
altered.

Edges also have a `traverse` function which will try to actually apply
the change which the edge implements to the system. This will usually
change the system state, but this isn't strictly required (e.g., there
can be edges in the graph which declare a dependency, but don't actually
make changes; the default Edge implementation is actually such an edge).

The return value of the `traverse` function has the same meaning as the
return value of the `estimate` function, except that `traverse`
functions which return a value not 0 or 1000 are weird.

If a `traverse` function returns 1000, the system will recalculate the
paths, and try again. Edges which returned 1000 on a `traverse` should
therefore return 1000 (or at the very least, not 0) at the next
`estimate`, otherwise ipcfg will probably start looping over trying to
traverse the same path all the time.

The edge has a property `is_down_edge`, which should be `false` for
edges that bring us closer to "we have working network", and which
should be `true` for edges that bring us farther away from that point.

Finally, the edge has properties for the node to which the `traverse`
function will move, and the node from which the traverse function is
moving. Edges in ipcfg are directed; ipcfg does not support undirected
edges. 

### Node

Nodes have a function `is_active` which simply returns whether the state
represented by this particular node is "current". This function should
not rely on any cache to optimize multiple `is_active` calls, unless it
has a way to figure out when (relevant) system state has changed and
invalidates its cache at that point.

Nodes also have read-only properties `in_edges` and `out_edges` which
return the edges that point to or from this particular node.

No function or method related to a node must ever change the system
state.

In any ipcfg graph, the "currently active" node is defined as whichever
node is active and has the highest number of (direct or indirect) active
nodes on the other ends of its in edges (i.e., the active node that is
on the high point of the food chain). For the purpose of this check,
"down" edges are ignored.

### Path

A path is a linked list of Path objects. Every Path object points to one
Edge (not Node) that needs to be traversed. The reason we point to edges
rather than nodes is because nodes may have multiple edges between them;
for instance, a node representing "wlan0 has no link" and one
representing "wlan0 has link" may have several edges, one for each
configured ESSID, and then finally one more for "any random ESSID".

Path has a "prev" member, a pointer to another Path object, which refers
to the previous node in this path, and an "out_paths" dynamic array,
referring to other paths that build upon this path.

The Path object keeps track of the cumulative return values of the
`estimate` functions that lead to its edge (starting at the "currently
active" node). It has a `repos` function which is called when a better
path is found to any of the nodes in its dependency chain, so that it
can update its own score (and then call the `repos` method on its out
paths).

There will be no two Path objects which end up at the same node. In
fact, Path objects are referred to by the node to which they describe a
path.

The actual decision code is in the Mapper class. It is possible (and
legal) to instanciate multiple Mapper objects; each would refer to a
different decision. This has two main methods: a `find_current` method
which will search for the "currently active" node as described above,
and a `map_paths` method which will find the shortest path to any node
in the system (starting from the currently active node).
