# debug log

- - -
## cold start / exc_return

task_launch was a naked function doing manual PSP setup + bl systick_init + bx
lr. Replaced entirely. Two problems with letting PendSV handle first launch
directly:

- current_task is NULL on boot, PendSV would try to save it
- LR on first entry points back to MSP, not PSP

Fix: scheduler_init sets current_task = NULL, next_task = head, pends PendSV.
PendSV checks cbz current_task — if NULL, skips context save, initializes
SysTick, manually loads LR = 0xFFFFFFFD, branches to first task. On all
subsequent switches this block is skipped; hardware EXC_RETURN is preserved
as-is for FPU safety.

- - -
## systick race condition

systick_init() was called in scheduler_init() before PSP was valid. SysTick
could fire in the window before first task launch → PendSV on null PSP →
HardFault.

Fix: systick_init() moved inside PendSV cold start path, after PSP is set and
ISB issued. SysTick cannot arm until PSP is valid.

- - -
## pendsv priority

PendSV must be set to lowest priority (0xFF) before SysTick is enabled. If
PendSV runs at higher priority than SysTick, a SysTick firing during PendSV
creates a preemption the scheduler isn't designed to handle.

- - -
## bug: svc parameter mismatch

symptom: MemManageHandler immediately after svc. SVC_Handler_c read garbage for
svc_type.

cause: manual push {lr} executed before capturing SP shifted the frame offset
by 4 bytes. C code indexed into the wrong slot and read old LR instead of PC.

fix: capture SP before touching the stack. When mixing hardware and software
stacking, order matters — capture SP exactly as hardware left it.

- - -
## bug: yield() not switching to next task

symptom: yield() before SysTick → HardFault. After SysTick → switched to
current task.

cause: SYS_YIELD pended PendSV without updating next_task first. PendSV ran on
stale/NULL pointer.

fix: SYS_YIELD calls scheduler_run() in kernel before pending PendSV.

- - -
## bug: uart output corruption (cccccccc...)

symptom: exactly 1ms (84,224 cycles) after startup, UART output degrades to
repeated chars.

cause: uart_puts is not thread-safe. SysTick fires mid-write, next task touches
the same head pointer.

fix: needs atomic lock around uart_puts. Mutex deferred to future phase.

- - -
## bug: nvic pointer arithmetic

symptom: manual USART1 IRQ 37 trigger drops into DefaultHandler.

cause: missing cast on NVIC_ISERx base address. Integer arithmetic gave
0xE000E101 instead of correct offset. CPU forced 32-bit alignment, wrote to
wrong register, triggered IRQ 5 instead of 37.

fix: cast base to (volatile uint32_t *) before offset — enforces pointer
scaling.

- - -
## bug: log2ceil(0) collapse

symptom: data+BSS MPU region collapsed to zero. MemManageHandler on first data
write.

cause: empty test binary has no .data/.bss symbols. LOG2CEIL(0) in linker
script evaluated to 32 bytes, smaller than actual layout.

fix: MAX(. - _sdata, 32) — guarantees minimum region size of 32 bytes.

- - -
## measurement: context switch latency (pendsv steady state)

raw dwt readings: sw_start_cyccnt = 84162 sw_end_cyccnt   = 84226

pendsv body: 64 cycles first launch: 186 cycles (dominated by bl systick_init —
function call + pipeline flush)

stmdb/ldmia (8 regs each) account for ~24 cycles. rest is loads and branches.

freertos reference is ~80 cycles but includes priority scheduling, stack
overflow checks, and trace hooks. not a direct comparison.

lazy stacking: LSPEN=1 at reset, no action needed. FPU context not paid in
PendSV unless PendSV itself uses the FPU.

- - -
## measurement: exception dispatch latency (logic analyzer)

setup: SparkFun TOL-18627, 24MHz, PulseView on PA9.

first dispatch:  250ns ~~(~~21 cycles) = 6 samples subsequent:          292ns~~ (~~
24 cycles) = 7 samples difference:          1 sample period (41.7ns) — within
resolution, may or may not be architectural uncertainty:         ±3 cycles
(±41.7ns) at 24MHz

cortex-m4 baseline: 12 cycles hardware entry + ~3 cycles cold flash fetch +
pipeline refill.

pre-ART readings (superseded — FLASH_ACR ICEN/DCEN/PRFTEN not yet enabled):
USART1 software-pended dispatch: 28 cycles @ 84MHz = 333ns PENDSVSET → handler
entry: ~10 cycles at 24MHz = 416ns

- - -
## gdb notes for preemptive systems

stepping is useless — halts CPU, freezes SysTick. use breakpoints on handler
entry + continue. variables read as 0 if breakpoint lands before the write.
remove all breakpoints before logic analyzer capture — GDB halt mid-toggle
corrupts timing.

- - -
## code decisions

- SCB_ICSR = (1 << 28) not |= — avoids read-modify-write race on ICSR
- naked ISR: full inline asm required, clobber list irrelevant
- ISB before WFI: prevents compiler reordering between start-read and sleep
- ISB after MSR PSP: ensures new PSP visible before pipeline refill

- - -
## trm notes (cortex-m4)

exception entry: 12 cycles baseline — hardware pushes
xPSR/PC/LR/R12/R3/R2/R1/R0, vector fetch and first instruction fetch run in
parallel with stack push 29 cycles with active FP context (lazy stacking —
reserves space, defers write) 10 cycles exception return (27 with active FP
context) 6 cycles tail chaining — skips full unstack/restack when next IRQ
already pending

LDM/STM: interruptible, EPSR saves progress. never use on peripheral registers
— mid-transfer restart causes double-read/double-write (e.g. UART RX clears on
read)

SDIV/UDIV: multi-cycle, interruptible. hardware abandons and restarts on return.

- - -
## open items

- phase 4 uart: printf-style refactor deferred to next semester
- re-measure exception entry with lazy stacking disabled to isolate contribution
- add macros for bit shifts in system_clock.c (magic numbers)
- stm32f1 blink archive ported, sitting in labs/archive/
