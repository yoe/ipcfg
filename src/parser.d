module ipcfg.parser;

import std.stdio;

class TokenException : object.Exception {
	private string partialtoken;
	this(string pt, string file = __FILE__, uint line = __LINE__) {
		partialtoken = pt;
		super("unknown token found.", file, line);
	}
	void prepend(string s) {
		partialtoken = s ~ partialtoken;
	}
	override string toString() {
		return "unknown token " ~ partialtoken ~ " found.";
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
				t.add_token(token[1..$], token_val);
			} else {
				children[token[0]] = new Tokenizer(token[1..$], token_val);
			}
		} else {
			tval = token_val;
		}
	}
	int get_tokenval(ref string stream) {
		try {
			if(tval) {
				return tval;
			}
			if(stream.length > 1) {
				return children[stream[0]].get_tokenval(stream[1..$]);
			}
		} catch (TokenException t) {
			t.prepend(stream[0..1]);
			throw t;
		}
		throw new TokenException(stream[0..1], __FILE__, __LINE__);
	}
}

class Parser {
	private File f;
	this(string filename) {
		f.open(filename);
	}
	void parse() {
		foreach(ubyte[] buf; f.byChunk(4096)) {
		}
	}
}
