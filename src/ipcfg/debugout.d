module ipcfg.debugout;

import std.stdio;

uint debuglevel = 0;

void setDebugLevel(uint level) {
	debuglevel = level;
}

template tdebug(alias func) {
	void tdebug(T...)(int level, T args) {
		if(level <= debuglevel) {
			func(args);
		}
	}
}

alias tdebug!(write) wdebug;
alias tdebug!(writef) wdebugf;
alias tdebug!(writeln) wdebugln;
alias tdebug!(writefln) wdebugfln;
