proc myCallback { processorName breakpointNumber } {
	# Get the value of the "status" variable on the application processor
	set lastStatus [idebugeval -processor cpu2_fprint -expression fprint]
	set fprint [lindex [lindex $lastStatus 0] 3]
	set status [string compare $fprint 3678047366,]
	# Log the value
	#puts "$processorName: GOT $lastStatus $fprint"
	# Stop if zero
	if { $status == 0 } {
	return True;
	}
}
