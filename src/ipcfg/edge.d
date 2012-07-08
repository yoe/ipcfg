module ipcfg.edge;

import ipcfg.node;
import ipcfg.debugout;

interface Edge {
	/*int estimate()
	  out(result) { assert((result >= 0) && (result <= 1000)); }
	int traverse()
	  out(result) { assert((result >= 0) && (result <= 1000)); }*/
	int estimate();
	int traverse();
	void retarget(Node to);
	void resource(Node from);
	bool parse_params(string);
	@property string stringof();
	@property uint cost();
	@property void cost(uint);
	@property Node to_node();
	@property Node from_node();
	@property bool is_down_edge();
}

class DefaultEdge : Edge {
	protected Node _to;
	protected Node _from;
	protected uint _cost = 1000;
	bool _is_down_edge = false;
	int estimate()
	  //out(result) { assert((result >= 0) && (result <= 1000)); }
	  body{
		if(_from.is_active()) {
			return 0;
		} else {
			return _cost > 1000 ? 1000 : _cost;
		}
	}
	int traverse()
	  //out(result) { assert((result >= 0) && (result <= 1000)); }
	  body{
	  	wdebugln(2, "traversing " ~ this.stringof);
		return estimate();
	  }
	this(Node from, Node to) {
		from.add_out_edge(this);
		to.add_in_edge(this);
		_to = to;
		_from = from;
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
	@property Node to_node() {
		return _to;
	}
	@property Node from_node() {
		return _from;
	}
	@property bool is_down_edge() {
		return _is_down_edge;
	}
	// this class doesn't support parameters
	bool parse_params(string s) {
		assert(s == "");

		return true;
	}
	void retarget(Node to) {
		_to.drop_in_edge(this);
		_to = to;
		_to.add_in_edge(this);
	}
	void resource(Node from) {
		_from.drop_out_edge(this);
		_from = from;
		_from.add_out_edge(this);
	}
}

class DefaultDownEdge : DefaultEdge {
	this(Node from, Node to) {
		super(from, to);
		_is_down_edge = true;
	}
	override int estimate() 
	  //out(result) { assert((result >= 0) && (result <= 1000)); }
	  body{
		if(_from.is_active()) {
			return _cost > 1000 ? 1000 : _cost;
		} else {
			return 0;
		}
	  }
}

class Loop : DefaultEdge {
	this(ipcfg.node.Node n) {
		super(n, n);
	}

	override int estimate() {
		return 0;
	}

	override @property string stringof() {
		return "Loop(" ~ _from.stringof ~ ")";
	}
}
