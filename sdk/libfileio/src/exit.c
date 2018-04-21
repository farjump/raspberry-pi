#define X_ASM_OPCODE(OPCODE) ASM_OPCODE(OPCODE)
#define ASM_OPCODE(OPCODE) \
   asm(" .int " #OPCODE "\n");

void _exit(int rc)
{
  (void) rc;
  X_ASM_OPCODE( K_GDB_BREAKPOINT_TRAP );
  while (1);
}
