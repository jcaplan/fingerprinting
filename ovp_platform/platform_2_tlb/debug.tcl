proc myCallback { processorName breakpointNumber } {
	# Get the value of the "status" variable on the application processor
	set lastStatus [idebugeval -processor cpu1_fprint -expression fprint]
	set fprint [lindex [lindex $lastStatus 0] 0]
	set status [string compare $fprint 836650139,]
	# Log the value
	#puts "$processorName: GOT $lastStatus"
	# Stop if zero
	puts $lastStatus
	if { $status == 0 } {
	return True;
	}
}
