#include "VC.h"

extern void VC_write32 ( unsigned int, unsigned int );
extern unsigned int VC_read32 ( unsigned int );

void VC_write32 ( unsigned int address , unsigned int value)
{
  *(unsigned int *)address = value;
  return;
}

unsigned int VC_read32 ( unsigned int address)
{
  return (*(unsigned int *)address);
}

unsigned int VC_MailboxWrite ( unsigned int mail_address, unsigned int channel )
{
  unsigned int mailbox;

  mailbox=0x2000B880;
  while(1)
  {
    if((VC_read32(mailbox+0x18)&0x80000000)==0) break;
  }
  VC_write32(mailbox+0x20,mail_address+channel);
  return(0);
}

unsigned int VC_MailboxRead ( unsigned int channel )
{
  volatile unsigned int ra;
  unsigned int mailbox;

  mailbox=0x2000B880;
  while(1)
  {
    while(1)
    {
      ra=VC_read32(mailbox+0x18);
      if((ra&0x40000000)==0) break;
    }
    ra=VC_read32(mailbox+0x00);
    if((ra&0xF)==channel) break;
  }
  return(ra);
}


int VC_init ( unsigned int horizontal , unsigned int vertical )
{
  unsigned int i;
  unsigned int frame_buffer;

  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[0], horizontal); /* #0 Physical Width */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[1], vertical);   /* #4 Physical Height */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[2], horizontal); /* #8 Virtual Width */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[3], vertical);   /* #12 Virtual Height */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[4], 0);          /* #16 GPU - Pitch */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[5], 32);         /* #20 Bit Depth */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[6], 0);          /* #24 X */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[7], 0);          /* #28 Y */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[8], 0);          /* #32 GPU - Pointer */
  VC_write32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[9], 0);          /* #36 GPU - Size */

  VC_MailboxWrite((K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[0]) | K_VC_GPU_MEMORY_OFFSET,1);
  VC_MailboxRead(1);

  /* Set frame_buffer to black */
  frame_buffer=VC_read32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[8]);
  frame_buffer &=(~ K_VC_GPU_MEMORY_OFFSET);
  for (i = 0; i < horizontal*vertical; i++)
  {
    VC_write32(frame_buffer,0xff000000);
    frame_buffer+=4;
  }
  return(0);
}


unsigned int VC_get_frame_buffer ()
{
  unsigned int frame_buffer;

  frame_buffer=VC_read32(K_VC_NOT_CACHEABLE_OFFSET + (unsigned int)&VC_aligned_buffer[8]);
  frame_buffer &=(~ K_VC_GPU_MEMORY_OFFSET);

  return frame_buffer;
}
