extern unsigned int   VC_aligned_buffer[];

#define K_VC_NOT_CACHEABLE_OFFSET  0x40000000
#define K_VC_GPU_MEMORY_OFFSET     0x80000000

extern int VC_init (unsigned int horizontal, unsigned int vertical);

extern unsigned int VC_get_frame_buffer ();
