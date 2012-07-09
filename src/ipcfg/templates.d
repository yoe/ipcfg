module ipcfg.templates;

import ipcfg.iface;
import ipcfg.node;
import ipcfg.edge;
import ipcfg.graph;

class InvalidPropertyException : Exception {
	this(string msg = "", string file = __FILE__, int line = __LINE__) {
		super(msg, file, line);
	}
}

private Template[string] templates;

Template getTemplateFor(string name) {
	if(!(name in templates)) {
		templates[name] = new Template();
	}
	return templates[name];
}

Node getDefaultNode(string s1, string s2) {
	Node n = new DefaultNode(s1);
	n.iface = s2;
	return n;
}

Edge getDefaultUpEdge(Node n1, Node n2) {
	return new DefaultEdge(n1, n2);
}

Edge getDefaultDownEdge(Node n1, Node n2) {
	return new DefaultDownEdge(n1, n2);
}

class Template {
	private TypeInfo _ti;
	private string _props[string];
	private Template[] _deps;

	private Node function(string, string) _creator = &getDefaultNode;
	private Edge function(Node, Node) _upedge = &getDefaultUpEdge;
	private Edge function(Node, Node) _downedge = &getDefaultDownEdge;

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

	@property Template[] deps() {
		return _deps;
	}

	void addDep(Template dep) {
		_deps ~= dep;
	}

	void setType(TypeInfo ti, Node function(string, string) creator) {
		_ti = ti;
		_creator = creator;
	}

	void setUpEdge(Edge function(Node, Node) upedge) {
		_upedge = upedge;
	}
	
	void setDownEdge(Edge function(Node, Node) downedge) {
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
