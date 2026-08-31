.section .init
.global _init
.type _init, @function
_init:
    push %ebp
    movl %esp, %ebp
    /* the contents of crtbegin.o's .init section will automatically go here 
       when we link compile the final kernel */

.section .fini
.global _fini
.type _fini, @function
_fini:
    push %ebp
    movl %esp, %ebp
    /* the contents of crtbegin.o's .fini section will automatically go here
       when we link compile the final kernel */

