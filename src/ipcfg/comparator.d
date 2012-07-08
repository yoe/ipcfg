module ipcfg.comparator;

import ipcfg.iface;
import ipcfg.node;
import ipcfg.edge;
import ipcfg.graph;

class InvalidPropertyException : Exception {
	this(string msg = "", string file = __FILE__, int line = __LINE__) {
		super(msg, file, line);
	}
}

class Comparator {
	private TypeInfo _ti;
	private string _props[string];
	private Comparator[] _deps;

	private Node function(string, string) _creator;
	private Edge function(Node, Node) _upedge;
	private Edge function(Node, Node) _downedge;

	@property bool hasProp(string name) {
		return ((name in _props) !is null);
	}

	string getProp(string name) {
		if(name in _props) {
			return _props[name];
		} else {
			throw new InvalidPropertyException;
		}
	}

	void setProp(string name, string val) {
		_props[name] = val;
	}

	@property TypeInfo type() {
		return _ti;
	}

	@property Comparator[] deps() {
		return _deps;
	}

	void addDep(Comparator dep) {
		_deps ~= dep;
	}

	void setType(TypeInfo ti, Node function(string, string) creator, Edge function(Node, Node) upedge, Edge function(Node, Node) downedge) {
		_ti = ti;
		_creator = creator;
		_upedge = upedge;
		_downedge = downedge;
	}

	Node instantiate(Iface i, Graph g) {
		Node depnode, newnode;
		newnode = _creator(_props["name"], i.name);
		foreach(dep; _deps) {
			depnode = g.getNodeWithContext(i, dep);
			_upedge(depnode, newnode);
			_downedge(newnode, depnode);
		}
		return newnode;
	}
}
