module ipcfg.node;

import ipcfg.edge;
import ipcfg.parser;

interface Node {
	bool is_active();
	bool parse_params(string);
	@property string stringof();
	void add_in_edge(Edge e);
	void add_out_edge(Edge e);
	@property Edge[] in_edges();
	@property Edge[] out_edges();
	@property bool wanted();
	@property void wanted(bool);
	string toString();
}

class DefaultNode : Node {
	private Edge[] _in_edges;
	private Edge[] _out_edges;
	private int score;
	private bool _wanted;

	bool is_active() {
		foreach(Edge e; _in_edges) {
			if(!e.from_node.is_active()) {
				return false;
			}
		}
		return true;
	}

	bool parse_params(string s) {
		assert(s == "");

		return true;
	}

	@property string stringof() {
		return "DefaultNode";
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

	void add_in_edge(Edge e) {
		_in_edges[_in_edges.length] = e;
	}

	void add_out_edge(Edge e) {
		_out_edges[_out_edges.length] = e;
	}

	string toString() {
		return "DefaultNode";
	}
}
