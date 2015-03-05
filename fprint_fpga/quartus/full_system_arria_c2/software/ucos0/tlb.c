#include <stdio.h>
#include "system.h"
#include "tlb.h"

unsigned int *LINE_ENABLE_REG = (unsigned int*)(TLB_BASE + (ENABLE_REG_OFFSET << 2));
unsigned int *LINE_VALID_REG = (unsigned int*)(TLB_BASE + (VALID_REG_OFFSET << 2));
unsigned int *VIRT_ADDR_TABLE = (unsigned int*)(TLB_BASE + (CPUTABLE_REG_OFFSET << 2));
unsigned int *PHYS_ADDR_TABLE = (unsigned int*)(TLB_BASE + (SPMTABLE_REG_OFFSET << 2));
unsigned int *TLB_ACTIVATE_REG = (unsigned int*)(TLB_BASE + (TLB_SEL_OFFSET << 2));
/*
 * Disables the TLB by setting its control register to 0.
 */
void disable_tlb() {
	*TLB_ACTIVATE_REG = 0x00000000;
}

/*
 * Enables the TLB by setting its control register to a non-zero value.
 */
void enable_tlb() {
	*TLB_ACTIVATE_REG = 0x00001000;
}

/*
 * Clears the entries in the CPU and SPM tables, starting with index 0
 * and ending before the position given by the 'count' variable.
 */
void reset_tlb(int count) {
	int i = 0;
	for(i = 0; i < count; i++) {
		*(VIRT_ADDR_TABLE + i) = 0;
		*(PHYS_ADDR_TABLE + i) = 0;
	}
}

/*
 * Checks if the TLB is on or off.
 */
unsigned int tlb_is_on() {
	return (*TLB_ACTIVATE_REG != 0x00000000);
}

/*
 * Turns on the selected bits in the Enable register.
 * Does not affect any other bits in the register.
 */
void set_enable(unsigned int line) {
	*LINE_ENABLE_REG = *LINE_ENABLE_REG | line;
}

/*
 * Turns off every bit in the Enable register.
 */
void reset_enable() {
	*LINE_ENABLE_REG = 0;
}

/*
 * Returns the contents of the Enable register as an unsigned integer.
 */
unsigned int get_enable() {
	return(*LINE_ENABLE_REG);
}

/*
 * Turns on the selected bits in the Valid register.
 * Does not affect any other bits in the register.
 */
void set_valid(unsigned int line) {
	*LINE_VALID_REG = *LINE_VALID_REG | line;
}

/*
 * Turns off every bit in the Valid register.
 */
void reset_valid() {
	*LINE_VALID_REG = 0;
}

/*
 * Returns the contents of the Valid register as an unsigned integer.
 */
unsigned int get_valid() {
	return(*LINE_VALID_REG);
}

/*
 * Takes an address, and writes its tag to a specific entry in the CPU table, where
 *'index' is the position that will be written to, relative to the address pointed
 * to by the CTRL_2 pointer (i.e. index 0 of the CPU table).
 */
void set_cputable_entry(int index, unsigned int address) {
	*(VIRT_ADDR_TABLE + index) = (address  >> PAGE_SIZE);

}

/*
 * Reads the contents of the CPU table at the specified index, relative to the
 * address pointed to by the CTRL_2 pointer.
 */
unsigned int get_cputable_entry(int index) {
	return *(VIRT_ADDR_TABLE + index);
}

/*
 * Takes an address, and writes its tag to a specific entry in the SPM table,
 * where 'index' is the position that will be written to, relative to the address
 * pointed to by the CTRL_3 pointer (i.e. index 0 of the SPM table).
 */
void set_spmtable_entry(int index, unsigned int address) {
	*(PHYS_ADDR_TABLE + index) = (address  >> PAGE_SIZE);
}

/*
 * Reads the contents of the SPM table at the specified index, relative to the
 * address pointed to by the CTRL_3 pointer.
 */
unsigned int get_spmtable_entry(int index) {
	return *(PHYS_ADDR_TABLE + index);
}
