# Roadmap

For the initial release, focus is made on **core kernel services**. **kernel debugging** facilities and a minimal **shell** will also be part of this release.

## Initial release / r-1.0.0

### r-0.0.1/ Minimal runtime

- [x] thread creation
- [x] Qemu riscv 64 bits support
- [x] Modular build system
- [x] Anckor test engine
- [x] Configure CI to run tests for each pull request

### r-0.0.2 / Syscalls

- [x] exception management
- [x] syscalls

### r-0.0.3 / Interrupts and IPC

- [x] Interrupt management
- [x] synchronous message-based IPC

### r-0.4.0 / Memory protection

- [x] independant module compilation
- [ ] User / Kernel stacks
- [ ] User / Kernel modes protection
- [ ] Memory protection between modules

### r-0.5.0 / Scheduling

- [ ] cooperative scheduling with min heap priority queue

## Backlog / Post r-1.0.0

- [ ] virtual memory management
- [ ] slab memory allocator
- [ ] asynchronous signal-based IPC
- [ ] serial driver
- [ ] gpio driver
- [ ] minimal version for ash (anckor shell)
- [ ] gdb server
- [ ] Qemu aarch64 support
- [ ] mutex support
- [ ] SMP support
- [ ] heap allocator (binary buddy system)
- [ ] Timer driver
- [ ] a convenient way to declare drivers for a platform (device tree ?)
- [ ] IDL support for inter-processes communication
- [ ] network stack
