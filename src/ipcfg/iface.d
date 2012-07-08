module ipcfg.iface;

import ipcfg.node;
import ipcfg.debugout;

class Iface {
	private string _name;
	private Node _topnode;
	private bool _hotplug;
	private bool _auto;

	this(string name) {
		_name = name;
		wdebug(5, "creating iface object");
	}

	@property Node topnode() {
		return _topnode;
	}

	@property void topnode(Node newnode) {
		if(_topnode !is null && _topnode.wanted) {
			_topnode.wanted = false;
			newnode.wanted = true;
		}
		_topnode = newnode;
		if(_hotplug || _auto) {
			_topnode.wanted = true;
		}
	}

	void markHotplug() {
		_hotplug = true;
		if(_topnode !is null) {
			_topnode.wanted = true;
		}
	}

	void markAuto() {
		_auto = true;
		if(_topnode !is null) {
			_topnode.wanted = true;
		}
	}
	
	@property string name() {
		return _name;
	}
}
