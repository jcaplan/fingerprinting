#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H

extern void context_switch(long *reg_stack);
extern void context_restore(long *reg_stack);

#endif
