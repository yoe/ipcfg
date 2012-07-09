#!/usr/bin/perl

my $counter = 0;
my $last;

while(<>) {
	$counter++;
	if (/Output parse/) {
		$counter = 0;
		$last = "parse";
	}
	if (/p.success/) {
		$counter = 0;
		$last = "success";
	}
	if (/return fail/) {
		$counter = 0;
		$last = "fail";
		print "{\nwriteln(\"failed \" ~ ruleName ~ \"\n\", p);\n";
	}
	if (/import std.array/) {
		print "import std.stdio;\n";
	}
	if($counter == 2) {
		if($last eq "parse") {
			print "writeln(\"attempting \" ~ ruleName);\n";
		}
		if($last eq "success") {
			print "writeln(\"\tMATCHED \" ~ ruleName ~ \"\n\", p);\n";
		}
		if($last eq "fail") {
			print "}\n";
		}
	}
	print $_;
}
