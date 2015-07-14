#
# mpu_utils_sw.tcl
#

# Create a new driver
create_sw_package mpu_utils
# Associate it with some hardware known as "altera_avalon_mutex"
# set_sw_property hw_class_name altera_avalon_mutex


# The version of this driver
set_sw_property version 13.1


# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory drivers

#
# Source file listings...
#

# C/C++ source files
add_sw_property c_source src/mpu_utils.c
# Include files
add_sw_property include_source inc/mpu_utils.h

# This driver supports HAL & UCOSII BSP (OS) types
add_sw_property supported_bsp_type HAL
add_sw_property supported_bsp_type UCOSII

# End of file
