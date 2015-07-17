/* MPU Specific Utilities Header file. */

#ifndef _MPU_UTILS_H_
#define _MPU_UTILS_H_

#include "system.h"
#include "nios2.h"
#include "sys/alt_exceptions.h"
 
#define DATA_REGION 1
#define INST_REGION 0

#ifdef NIOS2_MPU_PRESENT

/* Necessary Registers */

#define CONFIG_REG_REGNUM 13
#define MPUBASE_REG_REGNUM 14
#define MPUACC_REG_REGNUM 15
#define MPU_DATA_PERM_SUPER_NONE_USER_NONE 0
#define MPU_DATA_PERM_SUPER_RD_USER_NONE 1
#define MPU_DATA_PERM_SUPER_RD_USER_RD 2
#define MPU_DATA_PERM_SUPER_RW_USER_NONE 4
#define MPU_DATA_PERM_SUPER_RW_USER_RD 5
#define MPU_DATA_PERM_SUPER_RW_USER_RW 6
#define MPU_DATA_PERM_SZ 3
#define MPU_DATA_PERM_MASK 0x7
#define MPU_INST_PERM_SUPER_EXEC_USER_EXEC 2
#define MPU_INST_PERM_SUPER_EXEC_USER_NONE 1
#define MPU_INST_PERM_SUPER_NONE_USER_NONE 0
#define MPU_INST_PERM_SZ 2
#define MPU_INST_PERM_MASK 0x3
#define MPU_MAX_REGION_SIZE_LOG2 20
#define MPU_MAX_REGIONS 16
#define MPU_MIN_REGION_SIZE_LOG2 8
#define MPU_MIN_REGIONS 1
#define MPUACC_REG_C_LSB 5
#define MPUACC_REG_C_MSB 5
#define MPUACC_REG_C_SZ 1
#define MPUACC_REG_C_MASK 0x1
#define MPUACC_REG_LIMIT_LSB 8
#define MPUACC_REG_LIMIT_MSB 31
#define MPUACC_REG_LIMIT_SZ 24
#define MPUACC_REG_LIMIT_MASK 0xffffff
#define MPUACC_REG_LSB 0
#define MPUACC_REG_MASK_LSB 8
#define MPUACC_REG_MASK_MSB 30
#define MPUACC_REG_MASK_SZ 23
#define MPUACC_REG_MASK_MASK 0x7fffff
#define MPUACC_REG_MSB 31
#define MPUACC_REG_PERM_LSB 2
#define MPUACC_REG_PERM_MSB 4
#define MPUACC_REG_PERM_SZ 3
#define MPUACC_REG_PERM_MASK 0x7
#define MPUACC_REG_RD_LSB 1
#define MPUACC_REG_RD_MSB 1
#define MPUACC_REG_RD_SZ 1
#define MPUACC_REG_RD_MASK 0x1
#define MPUACC_REG_REGNUM 15
#define MPUACC_REG_RSV0_LSB 6
#define MPUACC_REG_RSV0_MSB 7
#define MPUACC_REG_RSV0_SZ 2
#define MPUACC_REG_RSV0_MASK 0x3
#define MPUACC_REG_RSV1_LSB 31
#define MPUACC_REG_RSV1_MSB 31
#define MPUACC_REG_RSV1_SZ 1
#define MPUACC_REG_RSV1_MASK 0x1
#define MPUACC_REG_SZ 32
#define MPUACC_REG_MASK 0xffffffff
#define MPUACC_REG_WR_LSB 0
#define MPUACC_REG_WR_MSB 0
#define MPUACC_REG_WR_SZ 1
#define MPUACC_REG_WR_MASK 0x1
#define MPUBASE_REG_BASE_LSB 8
#define MPUBASE_REG_BASE_MSB 30
#define MPUACC_REG_MASK_SZ 23
#define MPUACC_REG_MASK_MASK 0x7fffff
#define MPUACC_REG_MSB 31
#define MPUACC_REG_PERM_LSB 2
#define MPUACC_REG_PERM_MSB 4
#define MPUACC_REG_PERM_SZ 3
#define MPUACC_REG_PERM_MASK 0x7
#define MPUACC_REG_RD_LSB 1
#define MPUACC_REG_RD_MSB 1
#define MPUACC_REG_RD_SZ 1
#define MPUACC_REG_RD_MASK 0x1
#define MPUACC_REG_REGNUM 15
#define MPUACC_REG_RSV0_LSB 6
#define MPUACC_REG_RSV0_MSB 7
#define MPUACC_REG_RSV0_SZ 2
#define MPUACC_REG_RSV0_MASK 0x3
#define MPUACC_REG_RSV1_LSB 31
#define MPUACC_REG_RSV1_MSB 31
#define MPUACC_REG_RSV1_SZ 1
#define MPUACC_REG_RSV1_MASK 0x1
#define MPUACC_REG_SZ 32
#define MPUACC_REG_MASK 0xffffffff
#define MPUACC_REG_WR_LSB 0
#define MPUACC_REG_WR_MSB 0
#define MPUACC_REG_WR_SZ 1
#define MPUACC_REG_WR_MASK 0x1
#define MPUBASE_REG_BASE_LSB 8
#define MPUBASE_REG_BASE_MSB 30
#define MPUBASE_REG_BASE_SZ 23
#define MPUBASE_REG_BASE_MASK 0x7fffff
#define MPUBASE_REG_D_LSB 0
#define MPUBASE_REG_D_MSB 0
#define MPUBASE_REG_D_SZ 1
#define MPUBASE_REG_D_MASK 0x1
#define MPUBASE_REG_INDEX_LSB 1
#define MPUBASE_REG_INDEX_MSB 4
#define MPUBASE_REG_INDEX_SZ 4
#define MPUBASE_REG_INDEX_MASK 0xf
#define MPUBASE_REG_LSB 0
#define MPUBASE_REG_MSB 31
#define MPUBASE_REG_REGNUM 14
#define MPUBASE_REG_RSV0_LSB 5
#define MPUBASE_REG_RSV0_MSB 7
#define MPUBASE_REG_RSV0_SZ 3
#define MPUBASE_REG_RSV0_MASK 0x7
#define MPUBASE_REG_RSV1_LSB 31
#define MPUBASE_REG_RSV1_MSB 31
#define MPUBASE_REG_RSV1_SZ 1
#define MPUBASE_REG_RSV1_MASK 0x1
#define MPUBASE_REG_SZ 32
#define MPUBASE_REG_MASK 0xffffffff

#define STATUS_REG_MPU_LSB 0
#define STATUS_REG_MPU_MSB 1
#define STATUS_REG_MPU_RSV_LSB 2
#define STATUS_REG_MPU_RSV_MSB 31
#define STATUS_REG_MPU_RSV_SZ 30
#define STATUS_REG_MPU_RSV_MASK 0x3fffffff
#define STATUS_REG_MPU_SZ 2
#define STATUS_REG_MPU_MASK 0x3

#define MPU_DATA_REGION_VIOLATION_CAUSE_ID 17
#define MPU_DATA_REGION_VIOLATION_EXC_ID 19
#define MPU_INST_REGION_VIOLATION_CAUSE_ID 16
#define MPU_INST_REGION_VIOLATION_EXC_ID 18

#define BSTATUS_REG_MPU_LSB 0
#define BSTATUS_REG_MPU_MSB 1
#define BSTATUS_REG_MPU_SZ 2
#define BSTATUS_REG_MPU_MASK 0x3
#define BSTATUS_REG_REGNUM 2

#define ESTATUS_REG_MPU_LSB 0
#define ESTATUS_REG_MPU_MSB 1
#define ESTATUS_REG_MPU_SZ 2
#define ESTATUS_REG_MPU_MASK 0x3
#define ESTATUS_REG_REGNUM 1


/* Macros */

#define SET_NIOS2_STATUS_REG_PIE(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_STATUS_PIE_MSK)) | \
         (((Val) << NIOS2_STATUS_PIE_OFST) & NIOS2_STATUS_PIE_MSK) )

#define GET_NIOS2_STATUS_REG_PIE(Reg) \
    ( ((Reg) & NIOS2_STATUS_PIE_MSK) >> NIOS2_STATUS_PIE_OFST )

#define SzToMask(sz) \
        ((unsigned int)(((sz) == 32) ? 0xffffffff : ((0x1 << (sz)) - 1)))

#define GET_NIOS2_STATUS_REG_U(Reg) \
    ( ((Reg) & NIOS2_STATUS_U_MSK) >> NIOS2_STATUS_U_OFST )

#define SET_NIOS2_STATUS_REG_U(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_STATUS_U_MSK)) | \
         (((Val) << NIOS2_STATUS_U_OFST) & NIOS2_STATUS_U_MSK) )

// MPU Base PE bits
#define GET_NIOS2_CONFIG_REG_PE(Reg) \
    ( ((Reg) & NIOS2_CONFIG_REG_PE_MASK) >> NIOS2_CONFIG_REG_PE_OFST )

#define SET_NIOS2_CONFIG_REG_PE(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_CONFIG_REG_PE_MASK)) | \
         (((Val) << NIOS2_CONFIG_REG_PE_OFST) & NIOS2_CONFIG_REG_PE_MASK)  )

// MPU Base base addr bits
#define GET_NIOS2_MPUBASE_REG_BASE(Reg) \
    ( ((Reg) & NIOS2_MPUBASE_BASE_ADDR_MASK) >> NIOS2_MPUBASE_BASE_ADDR_OFST )

#define SET_NIOS2_MPUBASE_REG_BASE(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUBASE_BASE_ADDR_MASK)) | \
         (((Val) << NIOS2_MPUBASE_BASE_ADDR_OFST) & NIOS2_MPUBASE_BASE_ADDR_MASK) )

// MPU Base Index bits
#define GET_NIOS2_MPUBASE_REG_INDEX(Reg) \
    ( ((Reg) & NIOS2_MPUBASE_INDEX_MASK) >> NIOS2_MPUBASE_INDEX_OFST )

#define SET_NIOS2_MPUBASE_REG_INDEX(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUBASE_INDEX_MASK)) | \
         (((Val) << NIOS2_MPUBASE_INDEX_OFST) & NIOS2_MPUBASE_INDEX_MASK) )

// MPU  Base D bits
#define GET_NIOS2_MPUBASE_REG_D(Reg) \
    ( ((Reg) & NIOS2_MPUBASE_D_MASK) >> NIOS2_MPUBASE_D_OFST )

#define SET_NIOS2_MPUBASE_REG_D(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUBASE_D_MASK)) | \
         (((Val) << NIOS2_MPUBASE_D_OFST) & NIOS2_MPUBASE_D_MASK) )

// MPU ACC limit bits
#define GET_NIOS2_MPUACC_REG_LIMIT(Reg) \
    ( ((Reg) & NIOS2_MPUACC_LIMIT_MASK) >> NIOS2_MPUACC_LIMIT_OFST )

#define SET_NIOS2_MPUACC_REG_LIMIT(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUACC_LIMIT_MASK)) | \
         (((Val) << NIOS2_MPUACC_LIMIT_OFST) & NIOS2_MPUACC_LIMIT_MASK) )
         
// MPU ACC mask bits
#define GET_NIOS2_MPUACC_REG_MASK(Reg) \
    ( ((Reg) & NIOS2_MPUACC_MASK_MASK) >> NIOS2_MPUACC_MASK_OFST )

#define SET_NIOS2_MPUACC_REG_MASK(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUACC_MASK_MASK)) | \
         (((Val) << NIOS2_MPUACC_MASK_OFST) & NIOS2_MPUACC_MASK_MASK) )
       
// MPU ACC cache bits
#define GET_NIOS2_MPUACC_REG_C(Reg) \
    ( ((Reg) & NIOS2_MPUACC_C_MASK) >> NIOS2_MPUACC_C_OFST )

#define SET_NIOS2_MPUACC_REG_C(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUACC_C_MASK)) | \
         (((Val) << NIOS2_MPUACC_C_OFST) & NIOS2_MPUACC_C_MASK) )
         
// MPU ACC permissions bits
#define GET_NIOS2_MPUACC_REG_PERM(Reg) \
    ( ((Reg) & NIOS2_MPUACC_PERM_MASK) >> NIOS2_MPUACC_PERM_OFST )

#define SET_NIOS2_MPUACC_REG_PERM(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUACC_PERM_MASK)) | \
         (((Val) << NIOS2_MPUACC_PERM_OFST) & NIOS2_MPUACC_PERM_MASK) )
         
// MPU ACC read bit (write only)
#define SET_NIOS2_MPUACC_REG_RD(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUACC_RD_MASK)) | \
         (((Val) << NIOS2_MPUACC_RD_OFST) & NIOS2_MPUACC_RD_MASK)  )

// MPU ACC write bit (write only)
#define SET_NIOS2_MPUACC_REG_WR(Reg, Val) \
    Reg = ( ((Reg) & (~NIOS2_MPUACC_WR_MASK)) | \
         (((Val) << NIOS2_MPUACC_WR_OFST) & NIOS2_MPUACC_WR_MASK) )

/*
 * Add extra register read/write macros that aren't in nios2.h.
 */
#define NIOS2_WRITE_ESTATUS(src) \
        do { __builtin_wrctl(ESTATUS_REG_REGNUM, src); } while (0)

#define NIOS2_WRITE_BSTATUS(src) \
        do { __builtin_wrctl(BSTATUS_REG_REGNUM, src); } while (0)

#define NIOS2_WRITE_R2(r2) \
        do { __asm volatile ("mov r2, %z0" : : "rM" (r2)); } while (0)

#define NIOS2_WRITE_EA(ea) \
        do { __asm volatile ("mov ea, %z0" : : "rM" (ea)); } while (0)

#define NIOS2_WRITE_RA(ra) \
        do { __asm volatile ("mov ra, %z0" : : "rM" (ra)); } while (0)

#define NIOS2_READ_R2(r2) \
        do { __asm ("mov %0, r2" : "=r" (r2) ); } while (0)

#define NIOS2_READ_EA(ea) \
        do { __asm ("mov %0, ea" : "=r" (ea) ); } while (0)

#define NIOS2_READ_RA(ra) \
        do { __asm ("mov %0, ra" : "=r" (ra) ); } while (0)

#define NIOS2_NEXTPC_TO_RA() \
        do { __asm volatile ("nextpc ra"); } while (0)

#define NIOS2_ERET() \
        do { __asm volatile ("eret"); } while (0)

#define GET_REGISTER_VALUE(REG_NAME) \
        ({ Word __value; \
        asm volatile ( \
                "stw " REG_NAME ", %0" \
                : "=m"(__value) /* output register */ \
                : /* input register */ \
        ); \
        __value; \
        })

/* Enums */

enum Nios2MPUType
{
        MPU_DATA=1,
        MPU_INST=0
};
typedef struct
{
        unsigned int base;
        unsigned int index;
        unsigned int mask;
        unsigned int c;
        unsigned int perm;
} Nios2MPURegion;


/* Global data structure to hold exception information. */
typedef struct {
  int exceptions_serviced_with_handler;
  int mpu_data_exception_count;
  int mpu_inst_exception_count;
  int reissue_count;
  alt_u32 last_valid_badaddr;
  alt_u32 last_exception_pc;
  alt_exception_cause last_exception_cause;
  alt_exception_result desired_exception_result;
} exception_struct;

exception_struct g_exc_info;


/* Routines */

void nios2_mpu_init( unsigned int d );
void nios2_mpu_data_init();
void nios2_mpu_inst_init();
void nios2_mpu_load_region(Nios2MPURegion region[], unsigned int num_of_region, unsigned int d);
unsigned int nios2_mpu_get_mask(unsigned int region_size_log2, unsigned int d);
unsigned int nios2_mpu_get_limit(unsigned int limit_addr, unsigned int d);
void nios2_mpu_read_region(unsigned int index, unsigned int* base_ptr, unsigned int* mask_ptr, unsigned int* c_ptr, unsigned int* perm_ptr, unsigned int d);
void nios2_mpu_enable();
void nios2_mpu_disable();
void nios2_write_config(unsigned int pe);
void nios2_write_status(unsigned int u, unsigned int pie);
void nios2_read_status(unsigned int* u_ptr, unsigned int* pie_ptr);
void nios2_write_estatus(unsigned int u, unsigned int pie);
void nios2_read_estatus(unsigned int* u_ptr, unsigned int* pie_ptr);
void nios2_write_bstatus(unsigned int u, unsigned int pie);
void nios2_read_bstatus(unsigned int* u_ptr, unsigned int* pie_ptr);
void nios2_write_config(unsigned int pe);
void nios2_read_config(unsigned int* pe_ptr);
void nios2_write_mpubase(unsigned int base, unsigned int index, unsigned int d);
void nios2_read_mpubase(unsigned int* base_ptr, unsigned int* index_ptr, unsigned int* d_ptr);
void nios2_write_mpuacc(unsigned int mask, unsigned int c, unsigned int perm, unsigned int rd, unsigned int wr);
void nios2_read_mpuacc(unsigned int* mask_ptr, unsigned int* c_ptr, unsigned int* perm_ptr);
unsigned int sizeToLog2(unsigned int size);


#endif /* NIOS2_MPU_PRESENT */


#endif /* _MPU_UTILS_H */
