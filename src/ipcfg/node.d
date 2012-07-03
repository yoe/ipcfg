module ipcfg.node;

import ipcfg.edge;
import ipcfg.parser;

interface Node {
	bool is_active();
	@property string stringof();
	void add_in_edge(Edge e);
	void add_out_edge(Edge e);
	@property Edge[] in_edges();
	@property Edge[] out_edges();
	@property bool wanted();
	@property void wanted(bool);
}

class DefaultNode : Node {
	private Edge[] _in_edges;
	private Edge[] _out_edges;
	private int _score;
	private bool _wanted;
	private bool _loop;
	private string _name;

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
		return "DefaultNode(" ~ _name ~ ")";
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
