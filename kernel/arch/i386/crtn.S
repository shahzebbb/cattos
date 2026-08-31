.section .init
    /* the contents of crtend.o's .init section will automatically go here 
       when we link compile the final kernel */
    popl %ebp
    ret

.section .fini
    /* the contents of crtend.o's .fini section will automatically go here 
       when we link compile the final kernel */
    popl %ebp
    ret    
