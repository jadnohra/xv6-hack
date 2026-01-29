# Halt: Exiting QEMU from xv6

## Goal

Exit QEMU by typing a command in xv6 instead of `Ctrl-a x`.

## First Attempt: `exit` in sh.c

Added to shell's main loop:

```c
if(strcmp(buf, "exit\n") == 0)
  exit();
```

Shell exits. But `init` notices its child died and spawns a new shell. Can't escape — `init` always restarts.

## Second Attempt: `outw` in sh.c

QEMU exits when you write to its debug port:

```c
outw(0x604, 0x0);
```

Doesn't work. `outw` is a privileged I/O instruction — requires ring 0. The shell runs in ring 3. CPU raises an exception.

## Solution: A Syscall

User code must ask the kernel to do it.

```
sh.c            →  halt()           (user mode, ring 3)
                        ↓ trap
sys_halt()      →  outw(0x604, 0x0) (kernel mode, ring 0)
                        ↓
                   QEMU exits
```

Wire it like any syscall:
- `syscall.h`: `#define SYS_halt 23`
- `sysproc.c`: `sys_halt()` calls `outw(0x604, 0x0)`
- `syscall.c`: dispatch table entry
- `usys.S`: `SYSCALL(halt)`
- `user.h`: `int halt(void);`
- `halt.c`: user program that calls `halt()`

## Why

Two barriers:

1. **init respawns the shell.** Exiting the shell doesn't exit the system.

2. **I/O is privileged.** Only kernel can talk to hardware ports. User code must syscall.

The only way out is through ring 0.
