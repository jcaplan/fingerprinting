#define ANNOT_MAXITER(n) __asm__("1:\n\
        .section .wcet_annot\n \
        .long 1b\n\
        .long 1\n\
        .long " #n "\n\
        .text");

#endif
