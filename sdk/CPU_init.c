

/* This function initialise the MMU with the following mapping */
/*        Physical 0x00000000 - 0x20000000, virtual 0x00000000 - 0x20000000,   RWX user, RWX super, write-back, no write allocate */
/* RPI1   Physical 0x20000000 - 0x21000000, virtual 0x20000000 - 0x21000000,   RWX user, RWX super, write-back, no-cacheable */
/* RPI2&3 Physical 0x3F000000 - 0x40000000, virtual 0x20000000 - 0x21000000,   RWX user, RWX super, write-back, no-cacheable */
/*        Physical 0x00000000 - 0x40000000, virtual 0x40000000 - 0x80000000,   RWX user, RWX super, write-back, no-cacheable */

/* The CPU RAM size is limited to 512 to fit almost all raspberry */
/* The Whole RAM is accessible in non-cacheable mode from 0x40000000 to be able to access video RAM */


#define RPI1_IO_BASE_ADDRESS       0x20000000
#define RPI2_IO_BASE_ADDRESS       0x3F000000
#define RPI3_IO_BASE_ADDRESS       0x3F000000


/* number of 1M section in 4 GB address space */
#define NB_1M_SECTION (0x100000000LL / 0x00100000)
extern unsigned int CPU_init_page_table[NB_1M_SECTION];

/* */
#define ONE_MB  (1024 * 1024)
#define K_RAM_SIZE (512 *K_1M)

extern unsigned int CPU_init_read_main_id();
extern void CPU_init_stop_mmu();
extern void CPU_init_start_mmu(unsigned int * page_table,unsigned int control_register_or_mask);
extern void CPU_init_enable_vfp();
extern void CPU_init_invalidate_tlb();
extern void CPU_init_clean_and_invalidate_data_cache();
extern void CPU_init_invalidate_instruction_cache();
extern void CPU_init_disable_caches();

void CPU_init_map_section(
   unsigned int  *page_table,
   unsigned int   virtual_address,
   unsigned int   physical_address,
   unsigned int   flags)
{
   unsigned int   *entry;

   entry = &(page_table[virtual_address >> 20]);
   *entry = (physical_address & 0xFFF00000) | flags | 2 |0xC00;
   return;
}



void CPU_init()
{
   unsigned int main_id;
   unsigned int physical_io_address;
   unsigned int i;

   /* reading CPU id to known RPI version */
   main_id = CPU_init_read_main_id();

   /* rpi1 armv6 */
   if ((main_id & 0xF000) == 0xB000)
   {
      physical_io_address = RPI1_IO_BASE_ADDRESS;
   }
   /* rpi2 armv7 */
   else if ((main_id & 0xF000) == 0xC000)
   {
      physical_io_address = RPI2_IO_BASE_ADDRESS;
   }
   /* rpi3 armv8 */
   else if ((main_id & 0xF000) == 0xD000)
   {
      physical_io_address = RPI3_IO_BASE_ADDRESS;
   }

   /* clear page table */
   for (i = 0;i < NB_1M_SECTION; i++)
   {
      CPU_init_page_table[i] = 0;
   }

   /* map     Physical 0x00000000 - 0x20000000, virtual 0x00000000 - 0x20000000,   RWX user, RWX super, write-back, no write allocate */
   for (i = 0x00000000; i < (512 * ONE_MB); i+= ONE_MB)
   {
      CPU_init_map_section(CPU_init_page_table,i,i,0x0c); // cacheable no write allocate
   }

   /* RPI1   Physical 0x20000000 - 0x21000000, virtual 0x20000000 - 0x21000000,   RWX user, RWX super, write-back, no-cacheable */
   /* RPI2&3 Physical 0x3F000000 - 0x40000000, virtual 0x20000000 - 0x21000000,   RWX user, RWX super, write-back, no-cacheable */
   for (i = 0; i < (16 * ONE_MB) ; i+= ONE_MB)
   {
      CPU_init_map_section(CPU_init_page_table,RPI1_IO_BASE_ADDRESS + i,physical_io_address + i,0); // not cacheable
   }

   /* Map    Physical 0x00000000 - 0x40000000, virtual 0x40000000 - 0x80000000,   RWX user, RWX super, write-back, no-cacheable */
   for (i = 0x00000000; i < 0x40000000; i+= ONE_MB)
   {
      CPU_init_map_section(CPU_init_page_table,0x40000000 + i,i,0x0); // not cacheable
   }

//   clean_and_invalidate_data_cache();
//   invalidate_instruction_cache();
//   disable_caches();
   CPU_init_stop_mmu();
   CPU_init_invalidate_tlb();
   CPU_init_start_mmu((unsigned int *)((unsigned int)CPU_init_page_table),0x1005); /* ICACHE DCACHE and MMU ON */
   CPU_init_enable_vfp();

   return;
}
