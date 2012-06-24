module ipcfg.node;

import ipcfg.edge;

interface Node {
	bool is_active();
	bool parse_params(string);
	@property string stringof();
	void add_in_edge(Edge e);
	void add_out_edge(Edge e);
}

class DefaultNode : Node {
	private Edge[] in_edges;
	private Edge[] out_edges;

	bool is_active() {
		foreach(Edge e; in_edges) {
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

	void add_in_edge(Edge e) {
		in_edges[in_edges.length] = e;
	}

	void add_out_edge(Edge e) {
		out_edges[out_edges.length] = e;
	}
}
