
proc funcStart { processorName breakpointNumber } {

	if { ![info exists ::max_elapsed]} {
		set ::max_elapsed 0
	}
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
	if {$elapsed > $::max_elapsed} {
		set ::max_elapsed $elapsed
		puts "maximum elapsed: $::max_elapsed"
	}
	return False;
}
