module ipcfg.graph;

import ipcfg.node;
import ipcfg.edge;
import ipcfg.debugout;
import ipcfg.iface;
import ipcfg.templates;

import std.stdio;
import std.array;
import std.algorithm;

class UnknownStateException : Exception {
	this(string msg, string file = __FILE__, int line = __LINE__) {
		super(msg, file, line);
	}
}

class UnknownPathException : Exception {
	this(string msg, string file = __FILE__, int line = __LINE__) {
		super("Path description unknown:"~ msg, file, line);
	}
}

class Path {
	protected Path* prevpath;
	protected ipcfg.edge.Edge _edge;
	protected int _score;
	protected Path[] out_paths;
	protected bool _final;		/// Final means this path ends at a wanted node.
	protected Node[] _nodes;

	@property int score() {
		return _score;
	}

	@property bool Final() {
		return _final;
	}

	this(Path* prev, ipcfg.edge.Edge e) {
		if(prev !is null) {
			prevpath = prev;
			prev.add_out_path(this);
			_score = prevpath.score;
		}
		_edge = e;
		_score += e.estimate();
		if(e.to_node.wanted) {
			_final = true;
		} else {
			_final = false;
		}
		wdebugln(1, this.stringof ~ " created");
	}

	bool walk() {
		if(prevpath !is null) {
			wdebugln(1, "Attempting to walk " ~ this.stringof);
			if(!prevpath.walk()) {
				wdebugln(1, this.stringof ~ " not walked");
				return false;
			}
		}
		int rv;
		if((rv = _edge.traverse()) > 0) {
			wdebugln(1, "failed to traverse " ~ _edge.stringof ~ ": return value = ", rv);
			return false;
		}
		wdebugln(1, _edge.stringof ~ " traversed");
		return true;
	}

	void add_out_path(Path p) {
		out_paths ~= p;
	}

	void repos(Path p) {
		prevpath = &p;
		_score = p.score + _edge.estimate();
		foreach(Path po; out_paths) {
			po.repos(this);
		}
	}

	@property string stringof() {
		return "Path(" ~ _edge.to_node.stringof ~ ")";
	}
}

class InitialPath : Path {
	ipcfg.node.Node _node;
	override @property int score() {
		return 0;
	}

	override @property bool Final()  {
		return _final;
	}

	this(ipcfg.node.Node n) {
		_node = n;
		if(_node.wanted) {
			_final = true;
		} else {
			_final = false;
		}
		super(null, new ipcfg.edge.Loop(n));
	}

	override bool walk() {
		wdebugln(1,"Not walking the InitialPath, you're already there");
		return true;
	}
	override void repos(Path p) {
		throw new Exception("Invalid repositioning");
	}
	override @property string stringof() {
		return "InitialPath(" ~ _node.stringof ~ ")";
	}
}


class Graph {
	struct scored_wanted {
		ipcfg.node.Node node;
		int score;
	}

	private ipcfg.node.Node _nodes[];
	private ipcfg.iface.Iface[string] _ifaces;
	private Map _maps;
	private ipcfg.node.Node _currnode;
	private scored_wanted _wanted_nodes[];

	this(ipcfg.node.Node currnode) {
		_currnode = currnode;
	}

	void addWanted(ipcfg.node.Node w, int score) {
		scored_wanted sw;
		sw.node = w;
		sw.score = score;
		_wanted_nodes ~= sw;
	}

	void delWanted(ipcfg.node.Node w) {
		foreach(int i, scored_wanted sw; _wanted_nodes) {
			if(sw.node == w) {
				_wanted_nodes = _wanted_nodes[0..i-1] ~ _wanted_nodes[i+1..$];
				return;
			}
		}
	}

	ipcfg.iface.Iface iface(string name) {
		if(!(name in _ifaces)) {
			_ifaces[name] = new ipcfg.iface.Iface(name);
		}
		return _ifaces[name];
	}

	Node getNodeWithContext(Iface i, Template c) 
	out(result) { 
		assert(result.matches(c));
	}
	body {
		Node n;
		foreach(node; _nodes) {
			if(node.matches(c)) {
				return node;
			}
		}
		Iface newi;
		if(!c.hasProp("iface")) {
			newi = i;
		} else {
			newi = iface(c.getProp("name"));
		}
		n = c.instantiate(newi, this);
		_nodes ~= n;
		return n;
	}

	void fixNetwork() {
		ipcfg.node.Node start = currnode;
		sort!("a.score < b.score")(_wanted_nodes);
		addWanted(start, 0);
		foreach(scored_wanted sw; _wanted_nodes) {
			if(_maps is null || _maps.currNode != _currnode) {
				if(_maps !is null) {
					delete _maps;
				}
				_maps = new Map(_currnode);
				_maps.find_current();
				_currnode = _maps.currNode;
				_maps.map_paths();
			}
			_maps.get_path_for(sw.node).walk();
			if(sw.node.is_active()) {
				delWanted(start);
				return;
			}
		}
		delWanted(start);
	}
}

class Map {
	private ipcfg.node.Node _currnode;
	private ipcfg.node.Node _root;
	private Path[ipcfg.node.Node] _paths;
	private Path[string] _paths_by_string;
	private bool _have_current;

	this(ipcfg.node.Node currnode) {
		_currnode = currnode;
		_root = currnode;
	}

	@property ipcfg.node.Node rootNode() {
		return _root;
	}

	@property void rootNode(ipcfg.node.Node n) {
		_root = n;
	}

	@property ipcfg.node.Node currNode() {
		return _currnode;
	}

	/++
	 + Find the currently active state, by searching for a node which is
	 + active, but which has no out edges, or of which at least one node on
	 + the other end of an out edge is not active.
	 +/
	void find_current() {
		bool[ipcfg.node.Node] visited;
		ipcfg.node.Node candidate = _currnode;
		wdebugln(1, "Searching for the currently-active node");

		while(!_have_current) {
			bool have_out_active;
			wdebugln(1, "Considering ", _currnode.stringof);
			visited[_currnode] = true;
			if(_currnode.is_active()) {
				foreach(ipcfg.edge.Edge e; _currnode.out_edges) {
					if(!e.is_down_edge) {
						Node n = e.to_node;
						if(n.is_active()) {
							have_out_active = true;
							if(!(n in visited)) {
								candidate = e.to_node;
							}
						}
					}
				}
				if(!have_out_active) {
					wdebugln(1, "Currently active node: " ~ _currnode.stringof);
					_have_current = true;
					return;
				} else {
					wdebugln(1, "No, has active up edges");
				}
			} else {
				wdebugln(1, "No, is not active");
			}
			if(candidate == _currnode) {
				foreach(ipcfg.edge.Edge e; _currnode.in_edges) {
					if(!(e.from_node in visited)) {
						DefaultEdge de = cast(ipcfg.edge.DefaultEdge)(e);
						candidate = e.from_node;
					}
				}
			}
			if(candidate == _currnode) {
				throw new UnknownStateException(candidate.stringof);
			}
			_currnode = candidate;
		}
	}

	private void add_new_nodes(ref ipcfg.node.Node[] nodes, ipcfg.edge.Edge[] newedges, bool[ipcfg.node.Node] visited) {
		foreach(ipcfg.edge.Edge e; newedges) {
			Node n = e.to_node;
			if(!(n in visited)) {
				wdebugln(1, "Adding " ~ n.stringof ~ " to list of unprocessed nodes");
				nodes ~= n;
			} else {
				wdebugln(1, "Skipping " ~ n.stringof ~ ", already queued or processed");
			}
		}
	}

	/++
	 + This method will find the shortest paths to each and every node in
	 + the graph from what according to this class is the start graph.
	 +
	 + This method assumes that _currnode is the currently active node.
	 +/
	void map_paths() {
		bool[ipcfg.node.Node] visited;
		ipcfg.node.Node[] nodes_to_go;

		wdebugln(1, "Mapping paths from " ~ _currnode.stringof);

		assert(_have_current);
		_paths[_currnode] = new InitialPath(_currnode);
		visited[_currnode] = true;
		add_new_nodes(nodes_to_go, _currnode.out_edges, visited);

		int i;
		/* Can't use foreach here, since we modify nodes_to_go as part of the loop */
		for(i=0;i<nodes_to_go.length; i++) {
			ipcfg.node.Node n = nodes_to_go[i];
			if(n is null) {
				continue;
			}
			visited[n] = true;

			add_new_nodes(nodes_to_go, n.out_edges, visited);

			uint bestscore = cast(uint)-1;
			Path* bestpath;
			ipcfg.edge.Edge bestedge;

			foreach(ipcfg.edge.Edge e; n.in_edges) {
				ipcfg.node.Node from = e.from_node;
				if(!(from in visited)) {
					visited[from] = false;
				}
				if(visited[from]) {
					int s = _paths[from].score;
					if(s < bestscore) {
						bestscore = s;
						bestpath = &_paths[from];
						bestedge = e;
					}
				}
			}
			assert(bestpath !is null && bestedge !is null);

			_paths[n] = new Path(bestpath, bestedge);
			wdebugln(2, "Best path to ", n.stringof, ", score: ", _paths[n].score);
			foreach(ipcfg.edge.Edge e; n.out_edges) {
				if((e.to_node in _paths) !is null) {
					if(_paths[e.to_node].score > _paths[n].score + e.estimate()) {
						_paths[e.to_node].repos(_paths[n]);
					}
				}
			}
		}
	}

	Path get_path_for(Node n) {
		if(n in _paths) {
			return _paths[n];
		}
		throw new UnknownStateException("path to node " ~ n.stringof ~ " unknown");
	}

	Path get_path_by_descr(string descr) {
		if(descr in _paths_by_string) {
			return _paths_by_string[descr];
		} else {
			throw new UnknownPathException(descr);
		}
	}
}
