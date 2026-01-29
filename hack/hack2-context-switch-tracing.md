# Hack 2: Context Switch Tracing

## What We Did

Added a print in the scheduler to see every context switch.

## The Change

In `proc.c`, inside `scheduler()`, before `swtch()`:

```c
cprintf("%d -> %d\n", oldpid, p->pid);
```

## What We Saw

**Boot (one process):**
```
0 -> 1
0 -> 1
0 -> 1
```
Only `init` (pid 1) runs. No real switching.

**Shell starts:**
```
0 -> 2
0 -> 2
```
Shell (pid 2) is now the only runnable process.

**Multiple processes (`hackforkbomb`):**
```
0 -> 3
0 -> 4
0 -> 5
0 -> 6
0 -> 3
0 -> 4
0 -> 5
0 -> 6
```
Four processes, round-robin. Timer fires, scheduler picks next.

## Why 0 on the Left?

The scheduler isn't a process. When it runs, `c->proc` is 0. It picks a process, switches to it. Process runs until timer fires, yields back to scheduler.

## Primer Connection

From Part 7:

```
1. Timer interrupt fires
2. Save current process registers to memory
3. Load next process registers from memory
4. Return to next process
```

We're watching step 2-4 happen. The scheduler loop is the mechanism. Timer is the trigger.

## Files Touched

| File | Change |
|------|--------|
| `proc.c` | Added print in `scheduler()` |
| `hackspin.c` | Long-running process (spins) |
| `hackforkbomb.c` | Forks 4 copies of `hackspin` |

## xv6 → Linux Equivalent

| What | xv6 | Linux Kernel |
|------|-----|--------------|
| Scheduler function | `scheduler()` in `proc.c` | `schedule()` in `kernel/sched/core.c` |
| Context switch | `swtch()` in `swtch.S` | `context_switch()` in `kernel/sched/core.c` |
| Process state blob | `struct proc` in `proc.h` | `struct task_struct` in `include/linux/sched.h` |
| Register save/restore | `swtch.S` | `__switch_to()` in `arch/x86/kernel/process_64.c` |
| Run queue | Simple array `ptable.proc` | Per-CPU run queues, red-black trees (CFS) |

Same pattern, different scale:

| | xv6 | Linux |
|-|-----|-------|
| Scheduler | Round-robin, 50 lines | CFS + RT + deadline, 10,000 lines |
| Run queue | One array | Per-CPU red-black trees |
| Task struct | ~20 fields | ~700 fields |

## Tracing in Real Linux

Instead of adding prints, use built-in tracing:

```bash
# ftrace
echo 1 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable
cat /sys/kernel/debug/tracing/trace_pipe
```

Output:
```
bash-1234  [001] sched_switch: prev=bash:1234 next=chrome:5678
chrome-5678 [001] sched_switch: prev=chrome:5678 next=bash:1234
```

Same idea. Built-in, not hacked in.

## Key Insight

There's one CPU. Scheduler is a loop. Timer forces it to run. It picks the next RUNNABLE process and switches. That's it. Same in xv6, same in Linux.
