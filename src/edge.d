module ipcfg.edge;

import ipcfg.node;

interface Edge {
	int estimate()
	  out(result) { assert((result >= 0) && (result <= 1000)); }
	int traverse()
	  out(result) { assert((result >= 0) && (result <= 1000)); }
	bool parse_params(string);
	@property string stringof();
	@property uint cost();
	@property void cost(uint);
	@property void to_node(Node n);
	@property Node to_node();
	@property void from_node(Node n);
	@property Node from_node();
}

class DefaultEdge : Edge {
	private Node _to;
	private Node _from;
	private uint _cost = 1000;
	int estimate()
	  out(result) { assert((result >= 0) && (result <= 1000)); }
	  body{
		if(_to.is_active()) {
			return 0;
		} else {
			return _cost > 1000 ? 1000 : _cost;
		}
	}
	int traverse()
	  out(result) { assert((result >= 0) && (result <= 1000)); }
	  body{
		return estimate();
	  }
	@property string stringof() {
		return "DefaultEdge(" ~ _from.stringof() ~ " -> " ~ _to.stringof() ~ ")";
	}
	@property uint cost() {
		return _cost;
	}
	@property void cost(uint c) {
		_cost = c;
	}
	@property void to_node(Node n) {
		_to = n;
	}
	@property Node to_node() {
		return _to;
	}
	@property void from_node(Node n) {
		_from = n;
	}
	@property Node from_node() {
		return _from;
	}
	// this class doesn't support parameters
	bool parse_params(string s) {
		assert(s == "");

		return true;
	}
}

class DefaultDownEdge : DefaultEdge {
	int estimate() 
	  out(result) { assert((result >= 0) && (result <= 1000)); }
	  body{
		if(_to.is_active()) {
			return _cost > 1000 ? 1000 : _cost;
		} else {
			return 0;
		}
	  }
}

class Loop : DefaultEdge {
	this(ipcfg.node.Node n) {
		to_node = n;
		from_node = n;
		cost = 0;
	}
}
