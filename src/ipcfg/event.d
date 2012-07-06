module ipcfg.event;

interface EventParty {
	void eventReceived(Event);
}

class Event {
	private string _type;
	private string _msg;
	private EventParty _from;

	this(string type, string msg, EventParty from) {
		_type = type;
		_msg = msg;
		_from = from;
	}
	@property string type() {
		return _type;
	}
	@property string msg() {
		return _msg;
	}
	@property EventParty from() {
		return _from;
	}
}

class InvalidListenerException : Exception {
	this(string msg = "Listener not previously registered", string file =__FILE__, int line = __LINE__) {
		super(msg, file, line);
	}
}

class InvalidTypeException : Exception {
	this(string msg = "Type unknown", string file = __FILE__, int line = __LINE__) {
		super(msg, file, line);
	}
}

private EventParty listeners[string][];

void RegisterListener(string type, EventParty party) {
	if(type in listeners) {
		listeners[type] ~= party;
	} else {
		listeners[type][0] = party;
	}
}
void DeregisterListener(string type, EventParty party) {
	if(type in listeners) {
		foreach(int i, EventParty p; listeners[type]) {
			if(p == party) {
				listeners[type][i..($-1)] = listeners[type][i+1..$];
				listeners[type].length--;
				return;
			}
		}
		throw new InvalidListenerException;
	}
	throw new InvalidTypeException;
}

void IssueEvent(Event e, bool toSelf = false) {
	if(e.type in listeners) {
		foreach(EventParty p; listeners[e.type]) {
			if(p != e.from || !toSelf) {
				p.eventReceived(e);
			}
		}
	}
	foreach(EventParty p; listeners["*"]) {
		if(p != e.from || !toSelf) {
			p.eventReceived(e);
		}
	}
}
