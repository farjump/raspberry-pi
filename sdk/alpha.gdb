# Full set of registers as sent by Alpha
set tdesc filename sdk/armv6-core.xml

# Maximum exchange size of large packets
set remote memory-read-packet-size 1024
set remote memory-write-packet-size 1024

#
# Macro to make current function call back Alpha to restore its exception table.
# The macro replaces current function's return address (LR) with a program
# written at 0x2000 which calls Alpha and then returns to the actual current
# function's caller.
#
# Usage:
#   ```
#   tbreak *MY_EXCEPTION_TABLE_INIT_FUNCTION
#   command
#   alpha_hook_write_restore_exception_table
#   continue
#   end
#   ```
#   Note the use of GDB notation `*function` to set a breakpoint at the exact
#   entry address of `function`, before its prolog, because the macro must be used at
#   the exact entry point of the function that needs to be hooked.
#
define alpha_hook_write_restore_exception_table
  set $freemem        = (unsigned int*) 0x2000
  set $alpha_callback = (unsigned int) 0x07f09200

  # save the return address to the caller
  set *$freemem = $lr
  set $freemem += 1
  # save the callback address
  set *$freemem =  $alpha_callback
  set $freemem += 3

  # Set the return address to current freemem's address to return to it when
  # returning from current function. This is where the program will now be
  # written.
  set $lr = $freemem

  # Write the following program:
  # 0x2010:      str     r0, [pc, #-16]  ; 0x2008
  # 0x2014:      str     r1, [pc, #-16]  ; 0x200c
  # 0x2018:      ldr     r2, [pc, #-28]  ; 0x2004
  # 0x201c:      blx     r2
  # 0x2020:      ldr     r0, [pc, #-32]  ; 0x2008
  # 0x2024:      ldr     r1, [pc, #-32]  ; 0x200c
  # 0x2028:      ldr     r2, [pc, #-48]  ; 0x2000
  # 0x202c:      bx      r2
  #
  set *$freemem = 0xe50f0010
  set $freemem += 1
  set *$freemem = 0xe50f1010
  set $freemem += 1
  set *$freemem = 0xe51f201c
  set $freemem += 1
  set *$freemem = 0xe12fff32
  set $freemem += 1
  set *$freemem = 0xe51f0020
  set $freemem += 1
  set *$freemem = 0xe51f1020
  set $freemem += 1
  set *$freemem = 0xe51f2030
  set $freemem += 1
  set *$freemem = 0xe12fff12
  set $freemem += 1
end
