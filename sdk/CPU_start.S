        .text

        .globl CPU_start

CPU_start:
/* read ARM processor implemetation id */
        MRC     p15,0,r0,c0,c0,0
        MOV     r1,#0x0000F000
        AND     r1,r0,r1            

/* test if RPI1 */       
        MOV     r2,#0x0000B000
        CMP     r1,r2            
        BEQ     SKIP_RPI23

/* read current processsor status */
/* it should be in hyp so switch supervisor */        
        MRS        r0,cpsr
        BIC        r0,r0,#0xFF
        ORR        r0,r0,#0xD3
        MSR        spsr_cxsf,r0     
        ADD        r0,pc,#4
        .word    0xe12ef300
        //    MSR ELR_hyp,r0  , this register is not know when ARMv6 compiling
        .word    0xe160006e
        //ERET this instruction is not know when Armv6 compiling

SKIP_RPI23:
        LDR         sp, =__stack
        SUB         sp,sp,#64    
        BL          CPU_init    
        B           _start
