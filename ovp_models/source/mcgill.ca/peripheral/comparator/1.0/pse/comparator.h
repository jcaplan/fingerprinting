#include "peripheral/impTypes.h"

#define TASK_COUNTER                    0X0
#define START_POINTER                   0x1
#define END_POINTER                     0x2
#define OTHER                           0x3
#define CORE_ASSIGNMENT                 0x1
#define EXCEPTION                       0x0


#define COMPARATOR_CS                   0
#define COMPARATOR_CRC                  1
#define COMPARATOR_PAUSE                2
#define COMPARATOR_UNPAUSE              3


#define CH_IN                           0x0
#define CH_OUT                          0x1

typedef union COMP_CSR_SLAVE_address {
    Uns32 value;
        struct {
            unsigned offset       : 6;
            unsigned message_type : 2;
            unsigned core_id      : 4;
        } bits;
        struct {
            unsigned offset       : 2;
            unsigned pointer      : 4;
        } start,end;
        struct {
            unsigned offset       : 4;
            unsigned address      : 2;
        } reg;
} COMP_CSR_SLAVE_address;

typedef union exception_reg {
    Uns32 value;
        struct {
            unsigned task_id      : 4;
            unsigned overflow     : 1;
            unsigned error        : 1;
            unsigned irq          : 1;
            unsigned cpu_id       : 1;
        } bits;
} exception_reg;

typedef union COMP_FPRINT_SLAVE_address {
    Uns32 value;
        struct {
            unsigned offset       : 2;
            unsigned message_type : 4;
            unsigned core_id      : 4;
        } bits;
} COMP_FPRINT_SLAVE_address;

typedef union COMP_FPRINT_SLAVE_data {
    Uns32 value;
        struct {
            unsigned task_id : 4;
            unsigned status  : 1;
        } state;
        struct {
            unsigned core_no : 4;
            unsigned task_id : 4;
        } assignment;
} COMP_FPRINT_SLAVE_data;
