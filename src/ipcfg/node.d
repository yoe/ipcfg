module ipcfg.node;

import ipcfg.edge;

interface Node {
	bool is_active();
	@property string stringof();
	void add_in_edge(Edge e);
	void add_out_edge(Edge e);
	@property Edge[] in_edges();
	@property Edge[] out_edges();
	@property bool wanted();
	@property void wanted(bool);
	@property string iface();
	@property void iface(string);
}

class DefaultNode : Node {
	private Edge[] _in_edges;
	private Edge[] _out_edges;
	private int _score;
	private bool _wanted;
	private bool _loop;
	protected string _name;
	protected string _iface;

	bool is_active() {
		if(_loop) {
			return false;
		}
		_loop = true;
		foreach(Edge e; _in_edges) {
			if(!e.from_node.is_active()) {
				return false;
			}
		}
		_loop = false;
		return true;
	}

	this(string name) {
		_name = name;
	}

	@property string stringof() {
		string rv = typeof(this).stringof ~ "(" ~ _name;
		if(_iface.length > 0) {
			rv ~= ":" ~ _iface;
		}
		rv ~= ")";
		return rv;
	}

	@property Edge[] in_edges() {
		return _in_edges;
	}

	@property Edge[] out_edges() {
		return _out_edges;
	}

	@property bool wanted() {
		return _wanted;
	}

	@property void wanted(bool w) {
		_wanted=w;
	}

	@property int score() {
		return _score;
	}

	@property void score(int) {
		_score = score;
	}

	@property string iface() {
		return _iface;
	}

	@property void iface(string iface) {
		_iface = iface;
	}

	void add_in_edge(Edge e) {
		_in_edges ~= e;
	}

	void add_out_edge(Edge e) {
		_out_edges ~= e;
	}

	override bool opEquals(Object o) {
		Node n = cast(Node)(o);
		return (_in_edges == n.in_edges) && (_out_edges == n.out_edges) && (_wanted == n.wanted) && (this.stringof == n.stringof);
	}
}

class RootNode : DefaultNode {
	this(string name) {
		super(name);
	}
	override bool is_active() {
		return true;
	}

	/*override @property string stringof() {
		return "RootNode(" ~ _name ~ ")";
	}*/
}

version(unittest) {
	class AlwaysUpNode : DefaultNode {
		this() {
			super("Always Up testnode");
		}
		override bool is_active() {
			return true;
		}
	}

	class AlwaysDownNode : DefaultNode {
		this() {
			super("Always down testnode");
		}
		override bool is_active() {
			return true;
		}
	}
}

unittest {
	Node r = new AlwaysUpNode;
	Node c = new DefaultNode("child");
	Edge e = new DefaultEdge(r, c);
	Edge d = new DefaultDownEdge(c, r);
}
