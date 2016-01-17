proc myCallback { processorName breakpointNumber } {

	if { $breakpointNumber == 1 } {
		set ::start [igeticount]
		# puts "Start instruction: $::start"
	}

	if { $breakpointNumber == 2 } {
		set ::end [igeticount]
		# puts "End instruction: $::end"
		set elapsed [expr $::end - $::start]
		puts "PROFILE::TIME_ELAPSED: $elapsed"
	}
	return False
}


