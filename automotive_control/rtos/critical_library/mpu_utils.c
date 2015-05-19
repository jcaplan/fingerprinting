#include "mpu_utils.h"
#include "sys/alt_stdio.h"

/* MPU Routines */

#ifdef NIOS2_MPU_PRESENT

/* Design specific "safe" MPU data and instruciton initialization functions.
 *  - You'll want to customize these for your design.
 *  - All parameters for base and mask (limit or size mask) are in increments of minimum region size.
 *    - region_base    => base_byte_address/minimum_region_size
 *    - region_limit   => limit_byte_address/minimum_region_size
 *    - region_mask    => (region_address & region_mask) == region_base
 *
 *    Additional rules:
 *      - All regions must be aligned to powers of 2 boundaries.
 *      - All regions must be aligned to address boundaries in increments of region size. 
 *        - So, if a region's size is 4k, then its base address must be on integer multiples of 4k.
 */


void nios2_mpu_load_region(Nios2MPURegion region[],  unsigned int num_of_region, unsigned int d){
        unsigned int region_num;
        Nios2MPURegion current_region;

        for(region_num = 0; region_num < num_of_region; region_num++){
                current_region = region[region_num];

                nios2_write_mpubase(current_region.base /*base*/, current_region.index /*index*/, d /*d*/);
                nios2_write_mpuacc(current_region.mask /*mask*/, current_region.c /*c*/, current_region.perm /*perm*/, 0 /*rd*/, 1 /*wr*/);
        }
}

unsigned int nios2_mpu_get_mask(unsigned int region_size_log2, unsigned int d)
{
        unsigned int mask;
        unsigned int min_region_size_log2 = d ? NIOS2_MPU_MIN_DATA_REGION_SIZE_LOG2 : NIOS2_MPU_MIN_INST_REGION_SIZE_LOG2;
        unsigned int addr_width = d ? NIOS2_DATA_ADDR_WIDTH : NIOS2_INST_ADDR_WIDTH;

        if (region_size_log2 < min_region_size_log2)
                return -1;

        mask = (0x1FFFFFF << (region_size_log2 - MPUACC_REG_MASK_LSB)) &
                (SzToMask(addr_width) >> MPUACC_REG_MASK_LSB);

        return mask;
}

unsigned int nios2_mpu_get_limit(unsigned int limit_addr, unsigned int d)
{
        unsigned int limit;
        unsigned int min_region_addr = d ? NIOS2_MPU_MIN_DATA_REGION_SIZE : NIOS2_MPU_MIN_INST_REGION_SIZE;
        unsigned int addr_width = d ? NIOS2_DATA_ADDR_WIDTH : NIOS2_INST_ADDR_WIDTH;

        if (limit_addr < min_region_addr)
                return -1;

        limit = (SzToMask(addr_width + 1) & limit_addr) >> MPUACC_REG_LIMIT_LSB;

        return limit;
}

void nios2_mpu_enable(){
        //Enable MPU (will flush pipeline at the same time)
        nios2_write_config(1 /*pe*/);
}

void nios2_mpu_disable(){
        //Disable MPU
        nios2_write_config(0 /*pe*/);
}

unsigned int sizeToLog2(unsigned int size)
{
        unsigned int log2_value = -1;
        unsigned int i;
        for(i = 0; i < 32; i ++)
        {
                if ((1 << i) == size)
                        log2_value = i;
        }

        if(log2_value == -1)
                alt_printf("Unknown size value: %x\n", size);

        return log2_value;
}


void nios2_write_status(unsigned int u, unsigned int pie)
{
    unsigned int        status = 0;

    SET_NIOS2_STATUS_REG_U(status, u);
    SET_NIOS2_STATUS_REG_PIE(status, pie);

    NIOS2_WRITE_STATUS(status);
}

void nios2_read_status(unsigned int* u_ptr, unsigned int* pie_ptr)
{
    unsigned int        status;

    NIOS2_READ_STATUS(status);
    
    if (u_ptr) {
        *u_ptr = GET_NIOS2_STATUS_REG_U(status);
    }
    if (pie_ptr) {
        *pie_ptr = GET_NIOS2_STATUS_REG_PIE(status);
    }
}

void nios2_write_estatus(unsigned int u, unsigned int pie)
{
    unsigned int        estatus = 0;

    SET_NIOS2_STATUS_REG_U(estatus, u);
    SET_NIOS2_STATUS_REG_PIE(estatus, pie);

    NIOS2_WRITE_ESTATUS(estatus);
}

void nios2_read_estatus(unsigned int* u_ptr, unsigned int* pie_ptr)
{
    unsigned int        estatus;

    NIOS2_READ_ESTATUS(estatus);

    if (u_ptr) {
        *u_ptr = GET_NIOS2_STATUS_REG_U(estatus);
    }
    if (pie_ptr) {
        *pie_ptr = GET_NIOS2_STATUS_REG_PIE(estatus);
    }
}

void nios2_write_bstatus(unsigned int u, unsigned int pie)
{
    unsigned int        bstatus = 0;

    SET_NIOS2_STATUS_REG_U(bstatus, u);
    SET_NIOS2_STATUS_REG_PIE(bstatus, pie);

    NIOS2_WRITE_BSTATUS(bstatus);
}

void nios2_read_bstatus(unsigned int* u_ptr, unsigned int* pie_ptr)
{
    unsigned int        bstatus;

    NIOS2_READ_BSTATUS(bstatus);

    if (u_ptr) {
        *u_ptr = GET_NIOS2_STATUS_REG_U(bstatus);
    }
    if (pie_ptr) {
        *pie_ptr = GET_NIOS2_STATUS_REG_PIE(bstatus);
    }
}

void nios2_write_config(unsigned int pe)
{
    unsigned int        bstatus = 0;

    SET_NIOS2_CONFIG_REG_PE(bstatus, pe);
    NIOS2_WRITE_CONFIG(bstatus);
}

void nios2_read_config(unsigned int* pe_ptr)
{
    unsigned int        config;

    NIOS2_READ_CONFIG(config);

    if (pe_ptr) {
        *pe_ptr = GET_NIOS2_CONFIG_REG_PE(config);
    }
}

void nios2_write_mpubase(unsigned int base, unsigned int index, unsigned int d)
{
    unsigned int        mpubase = 0;

    SET_NIOS2_MPUBASE_REG_BASE(mpubase, base);
    SET_NIOS2_MPUBASE_REG_INDEX(mpubase, index);
    SET_NIOS2_MPUBASE_REG_D(mpubase, d);
    NIOS2_WRITE_MPUBASE(mpubase);
}

void nios2_read_mpubase(unsigned int* base_ptr, unsigned int* index_ptr, unsigned int* d_ptr)
{
    unsigned int        mpubase;

    NIOS2_READ_MPUBASE(mpubase);

    if (base_ptr) {
        *base_ptr = GET_NIOS2_MPUBASE_REG_BASE(mpubase);
    }
    
    if (index_ptr) {
    	*index_ptr = GET_NIOS2_MPUBASE_REG_INDEX(mpubase);
    }
    
    if (d_ptr) {
    	*d_ptr = GET_NIOS2_MPUBASE_REG_D(mpubase);
    }
}

void nios2_write_mpuacc(unsigned int mask, unsigned int c, unsigned int perm, unsigned int rd, unsigned int wr)
{
    unsigned int        mpuacc = 0;
    
#ifdef NIOS2_MPU_REGION_USES_LIMIT
    SET_NIOS2_MPUACC_REG_LIMIT(mpuacc, mask);
#else
    SET_NIOS2_MPUACC_REG_MASK(mpuacc, mask);
#endif
    SET_NIOS2_MPUACC_REG_C(mpuacc, c);
    SET_NIOS2_MPUACC_REG_PERM(mpuacc, perm);
    SET_NIOS2_MPUACC_REG_RD(mpuacc, rd);
    SET_NIOS2_MPUACC_REG_WR(mpuacc, wr);
    NIOS2_WRITE_MPUACC(mpuacc);
}

void nios2_read_mpuacc(unsigned int* mask_ptr, unsigned int* c_ptr, unsigned int* perm_ptr)
{
    unsigned int        mpuacc;

    NIOS2_READ_MPUACC(mpuacc);

    if (mask_ptr) {
#ifdef NIOS2_MPU_REGION_USES_LIMIT
    	*mask_ptr = GET_NIOS2_MPUACC_REG_LIMIT(mpuacc);
#else
        *mask_ptr = GET_NIOS2_MPUACC_REG_MASK(mpuacc);
#endif
    }
    
    if (c_ptr) {
        *c_ptr = GET_NIOS2_MPUACC_REG_C(mpuacc);
    }
    
    if (perm_ptr) {
        *perm_ptr = GET_NIOS2_MPUACC_REG_PERM(mpuacc);
    }
}

#endif /* NIOS2_MPU_PRESENT */
