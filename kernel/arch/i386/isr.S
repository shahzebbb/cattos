.global isr_stub_3

isr_stub_3:
    pusha
    call exception_handler
    popa
    iret
