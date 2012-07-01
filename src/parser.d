module ipcfg.parser;

import std.stdio;

class TokenException : object.Exception {
	private string partialtoken;
	private string msg;
	this(string pt, string msg, string file = __FILE__, uint line = __LINE__) {
		partialtoken = pt;
		super(msg ~ " found.", file, line);
	}
	void prepend(string s) {
		partialtoken = s ~ partialtoken;
	}
	override string toString() {
		return msg ~ partialtoken ~ " found.";
	}
}

class UnknownTokenException : TokenException {
	this(string pt, string msg = "unknown token", string file = __FILE__, uint line = __LINE__) {
		super(pt, msg, file, line);
	}
}

class DuplicateTokenException : TokenException {
	this(string pt, string msg = "duplicate token", string file = __FILE__, uint line = __LINE__) {
		super(pt, msg, file, line);
	}
}

class Tokenizer {
	private Tokenizer[char] children;
	private int tval = 0;
	this(string s, int token_val) {
		add_token(s, tval);
	}
	void add_token(string token, int token_val) {
		if(token.length > 1) {
			Tokenizer* t = (token[0] in children);
			if(t !is null) {
				try {
					t.add_token(token[1..$], token_val);
				} catch (DuplicateTokenException d) {
					e.prepend(token[0..1]);
				}
			} else {
				children[token[0]] = new Tokenizer(token[1..$], token_val);
			}
		} else {
			if(tval) {
				throw new DuplicateTokenException(s);
			}
			tval = token_val;
		}
	}
	int get_tokenval(string stream = "") {
		try {
			if(tval) {
				return tval;
			}
			if(stream.length > 1) {
				return children[stream[0]].get_tokenval(stream[1..$]);
			}
		} catch (UnknownTokenException t) {
			t.prepend(stream[0..1]);
			throw t;
		}
		throw new UnknownTokenException(stream[0..1], __FILE__, __LINE__);
	}
	Tokenizer get_child_token(char n) {
		if(children[n]) {
			return children[n];
		}
		throw new UnknownTokenException(n);
	}
	@property bool is_final() {
		if(tval) {
			return true;
		}
		return false;
	}
}

class Parser {
	private File f;
	this(string filename) {
		f.open(filename);
	}
	void parse() {
		Tokenizer t = new Tokenizer("", 0)
		t.add_token("auto", 1024);
		t.add_token("iface", 1025);
		t.add_token("	", 1026);
		t.add_token("inet", 1027);
		t.add_token("dhcp", 1028);
		Tokenizer ct = t;
		foreach(ubyte[] buf; f.byChunk(4096)) {
			foreach(ubyte b; buf) {
				try {
					ct = ct.get_child_token(b);
					if(ct.is_final) {
						std.writeln(ct.get_tokenval());
						ct = t;
					}
				} catch (TokenException e) {
					std.writeln("E: " ~ e.toString());
				}
			}
		}
	}
}

unittest {
	Parser p("");
}
