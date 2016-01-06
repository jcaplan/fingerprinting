MEMORY
{
    memory_0_onchip_memoryMain_BEFORE_RESET : ORIGIN = 0x400000, LENGTH = 204800
    reset : ORIGIN = 0x464000, LENGTH = 32
    memory_0_onchip_memoryMain : ORIGIN = 0x464020, LENGTH = 196576
    stack_bin_1 : ORIGIN = 0x494000, LENGTH = 4096
    stack_bin_0 : ORIGIN = 0x495000, LENGTH = 4096
    processor0_0_scratchpad_0 : ORIGIN = 0x4200000, LENGTH = 16384
}
