        .text

        .globl CPU_init_stop_mmu
        .globl CPU_init_start_mmu
        .globl CPU_init_enable_vfp
        .globl CPU_init_invalidate_tlb
        .globl CPU_init_clean_and_invalidate_data_cache
        .globl CPU_init_invalidate_instruction_cache
        .globl CPU_init_disable_caches
        .globl CPU_init_read_main_id
        

CPU_init_read_main_id:
        MRC    p15,0,r0,c0,c0,0
        BX     lr    
        

CPU_init_clean_and_invalidate_data_cache:
        // flush data cache
loop:   
        MOV         r2,#0
        MCR         p15,0,r2,c7,c10,0
        MRC         p15,0,r2,c7,c10,6
        ANDS        r2,r2,#01
        BEQ         done
        B           loop
done:
        // invalidate data cache */
        MCR         p15,0,r2,c7,c6,0
        MCR         p15,0,r2,c7,c10,4
        BX          lr

        

CPU_init_invalidate_instruction_cache:    
        MOV         r2,#0
        MCR         p15,0,r2,c7,c5,0
        MCR         p15,0,r2,c7,c10,4
        BX          lr


CPU_init_disable_caches:  
        MRC         p15,0,r2,c1,c0,0
        BIC         r2,#0x1000
        BIC         r2,#0x0004
        MCR         p15,0,r2,c1,c0,0
        BX          lr


CPU_init_stop_mmu:
        // disable mmu 
        MRC    p15,0,r2,c1,c0,0
        BIC    r2,#0x01
        MCR    p15,0,r2,c1,c0,0
        BX     lr
        
CPU_init_start_mmu:
//set domain register to 0x55555555 to allow client access  to perform check        
        LDR    r2,=0x55555555
        MCR    p15,0,r2,c3,c0,0 ;@ domain
// clear TTBC to enable only TTBR0
        MOV    r2,#0    
        MCR    p15,0,r2,c2,c0,2 ;@ TTBC
// set TTBR0 to pagetable address
        MRC    p15,0,r4,c2,c0,0 ;@ tlb base
        MCR    p15,0,r0,c2,c0,0 ;@ tlb base
        MCR    p15,0,r0,c2,c0,1 ;@ tlb base
        MOV    r2,#0
        MCR    p15,0,r2,c7,c10,4

        // write in control register input value
        MRC    p15,0,r2,c1,c0,0
        ORR    r2,r2,r1
        MCR    p15,0,r2,c1,c0,0
        BX     lr
        

        /* enable copro single doucle precision */
        /* enable float */
        /* set fpscr to flush to zero */
CPU_init_enable_vfp:
        //@ en@ enable the FPU
        MRC     p15, 0, r0, c1, c0, 2
        ORR     r0, r0, #0x300000            /* single precision */
        ORR     r0, r0, #0xC00000            /* double precision */
        MCR     p15, 0, r0, c1, c0, 2
        MOV     r0, #0x40000000
        FMXR    fpexc,r0
        MOV     r0,#0x01000000
        FMXR    fpscr,r0
        BX      lr
        
        
CPU_init_invalidate_tlb:
        MOV         r2,#0
        MCR         p15,0,r2,c8,c7,0 
        MCR         p15,0,r2,c7,c10,4
        BX          lr

        .section .page_table,"a"
        .align   14
        .globl   CPU_init_page_table
CPU_init_page_table:
        .space  (0x1000 * 4)
