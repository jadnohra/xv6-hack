# Hack 1: Add a Syscall — `getnumsyscalls()`

## What We Did

Added a syscall that returns how many syscalls have been made since boot.

## The Path

```
hacktest.c:     int n = getnumsyscalls();
                        │
                        ▼
usys.S:         movl $22, %eax
                int $0x40           ← trap instruction
                ret
                        │
         ══════════════════════════ TRAP (ring 3 → ring 0)
                        │
                        ▼
syscall.c:      syscalls[22]()      ← dispatch table lookup
                        │
                        ▼
sysproc.c:      sys_getnumsyscalls() { return total_syscalls; }
                        │
                        ▼
                result in %eax, return to user
```

## Files Touched

| File | What we added |
|------|---------------|
| `syscall.h` | `#define SYS_getnumsyscalls 22` |
| `sysproc.c` | `total_syscalls` counter + `sys_getnumsyscalls()` |
| `syscall.c` | Dispatch table entry + increment counter |
| `usys.S` | `SYSCALL(getnumsyscalls)` — user-side trap stub |
| `user.h` | `int getnumsyscalls(void);` declaration |
| `hacktest.c` | Test program |

## Mapping to Primer Sections

| File | Primer section |
|------|----------------|
| `syscall.h` | Part 4 — syscall number |
| `usys.S` | Part 4 — "syscall instruction" |
| `trap.c` | Part 3 — the controlled gate |
| `syscall.c` | Part 4 — dispatch table |
| `sysproc.c` | Part 4 — "kernel does actual work" |

## xv6 → Linux Equivalent

| What | xv6 | Linux Kernel |
|------|-----|--------------|
| Syscall number | `syscall.h` | `arch/x86/entry/syscalls/syscall_64.tbl` |
| Kernel function | `sysproc.c` | `kernel/sys.c` |
| Dispatch wiring | `syscall.c` array | Auto-generated from `.tbl` |
| User stub | `usys.S` | Not in kernel — libc does it |
| User declaration | `user.h` | `include/linux/syscalls.h` |
| IDT setup | `tvinit()` in `trap.c` | `arch/x86/kernel/idt.c` |
| Trap entry | `trapasm.S` | `arch/x86/entry/entry_64.S` |

## Result

```
$ hacktest
syscalls so far: 122

$ hacktest
syscalls so far: 160

$ hacktest
syscalls so far: 198
```

Counter grows ~38 per run — that's fork + exec + wait + write + exit from shell and test program.

## Key Insight

Same pattern in Linux, more indirection. xv6 is ~6,000 lines, Linux is ~30 million. The concepts are identical: syscall number → trap → dispatch table → kernel function → return.
