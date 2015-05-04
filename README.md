# fingerprinting
This project contains all things related to the development of a multicore Nios platform with fingerprinting

#fprint_fpga
This folder contains the Verilog source code for the platform as well as the Quartus generated HDL, project files, and software templates for each platform.
Future software development should use the BSPs but otherwise be placed in another folder.

#ovp_platform
The associated platform models for those found in fprint_fpga to simulate using OVP.

#ovp_models
Backup copies of the peripheral models used by the platforms.

#code_gen
Code generation project to map applications onto platforms and generate code for all cores using uCOS RTOS.

#crc_gen
Generate arbitrary CRC circuits in VHDL

#ctrl_systems
Design example of automotive control system generated in Matlab and port to Nios multicore system. Design project of Anthony Delage, Alexander Lunan, Gregory Williamson.