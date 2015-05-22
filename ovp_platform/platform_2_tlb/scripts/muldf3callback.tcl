proc funcStart { processorName breakpointNumber } {


	set ::count 0
	set ::start [igeticount]
	return False
}

proc loopEntry { processorName breakpointNumber } {

	set ::count [expr $::count + 1]
	return false
}

proc funcEnd  { processorName breakpointNumber } {

	set ::end [igeticount]
	set elapsed [expr $::end - $::start]
	puts "number of loops: $::count, total instructions: $elapsed"
	return False;
}

