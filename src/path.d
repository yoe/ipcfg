module ipcfg.path;

import ipcfg.node;
import ipcfg.edge;

class Path {
	private Path* prevpath;
	private ipcfg.edge.Edge _edge;
	private int _score;
	private Path[] out_paths;
	private bool _complete;		/// Complete means we've started at a node which is active.
	private bool _final;		/// Final means this path ends at a wanted node.

	@property int score() {
		return _score;
	}

	@property bool complete() {
		return _complete;
	}

	@property bool final() {
		return _final;
	}

	this(Path* prev, ipcfg.edge.Edge e) {
		if(prev !is null) {
			prevpath = prev;
			prev.add_out_path(this);
			_score = prevpath.score;
			_complete = prevpath.complete;
		} else {
			_complete = e.from_node.is_active();
		}
		_edge = e;
		_score += e.estimate();
		if(e.to_node.wanted) {
			_final = true;
		} else {
			_final = false;
		}
	}

	bool walk() {
		if(prevpath !is null) {
			if(!prevpath.walk()) {
				return false;
			}
		}
		if(_edge.traverse() > 0) {
			return false;
		}
		return true;
	}

	void abort() {
		_edge.abort();
		if(prevpath !is null) {
			prevpath.abort();
		}
	}

	void add_out_path(Path p) {
		out_paths[out_paths.length] = p;
	}

	void repos(Path p) {
		prevpath = p;
		_score = p.score + _edge.estimate();
		_complete = p.complete;
		foreach(Path po; out_paths) {
			po.repos(this);
		}
	}
}

class Mapper {
	private ipcfg.node.Node _graph;
	private ipcfg.node.Node[] _wanted_nodes;
	private Path[ipcfg.node.Node] _paths;
	private bool _have_current;

	this(ipcfg.node.Node graph) {
		_graph = graph;
	}

	void add_new_nodes(ref ipcfg.node.Node[] nodes, ipcfg.edge.Edge[] newedges, bool[ipcfg.node.Node] visited) {
		foreach(ipcfg.edge.Edge e; newedges) {
			if(!visited[e.to_node]) {
				nodes[nodes.length] = e.to_node;
			}
		}
	}

	/++
	 + Find the currently active state, by searching for a node which is
	 + active, but which has no out edges, or of which at least one node on
	 + the other end of an out edge is not active.
	 +/
	void find_current() {
		bool[ipcfg.node.Node] visited;
		ipcfg.node.Node candidate = _graph;

		while(true) {
			bool have_out;
			visited[_graph] = true;
			if(_graph.is_active()) {
				foreach(ipcfg.edge.Edge e; _graph.out_edges) {
					have_out = true;
					if(!e.to_node.is_active()) {
						_have_current = true;
						return;
					} else {
						if(!visited[e.to_node]) {
							candidate = e.to_node;
						}
					}
				}
				if(!have_out) {
					_have_current = true;
					return;
				}
			}
			if(candidate == _graph) {
				foreach(ipcfg.edge.Edge e; _graph.in_edges) {
					if(!visited[e.from_node]) {
						candidate = e.from_node;
					}
				}
			}
			if(candidate == _graph) {
				throw new UnknownStateException;
			}
			_graph = candidate;
		}
	}

	/++
	 + This method will find the shortest paths to each and every node in
	 + the graph from what according to this class is the start graph.
	 +
	 + This method assumes that _graph is the currently active node.
	 +/
	void map_paths() {
		bool[ipcfg.node.Node visited;
		ipcfg.node.Node[] nodes_to_go;

		assert(_have_current);
		nodes_to_go[0] = _graph;
		paths[_graph] = new ipcfg.edge.Loop(_graph);
		foreach(ipcfg.node.Node n; nodes_to_go) {
			visited[n] = true;

			add_new_nodes(nodes_to_go, n.out_edges, visited);

			uint bestscore = cast(uint)-1;
			Path* bestpath;
			ipcfg.edge.Edge bestedge;

			foreach(ipcfg.edge.Edge e; n.in_edges) {
				ipcfg.node.Node from = e.from_node;
				if(visited[from]) {
					int s = paths[from].score;
					if(s < bestscore) {
						bestscore = s;
						bestpath = paths[from];
						bestedge = e;
					}
				}
			}
			assert(bestpath !is null && bestedge !is null);

			paths[n] = new Path(bestpath, bestedge);
			foreach(ipcfg.edge.Edge e; n.out_edges) {
				if((e.to_node in paths) !is null) {
					if(paths[e.to_node].score > paths[n].score + e.to_node.estimate()) {
						paths[e.to_node].repos(paths[n]);
					}
				}
			}
		}
	}
}
