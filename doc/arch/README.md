# Architecture Design Records

This repository contains all design decisions in form of small [architecture design records](https://cognitect.com/blog/2011/11/15/documenting-architecture-decisions).

As a remainder, these **records** contain the following fields:
- a **title** section which describes briefly the subject of the note
- a **status** section which can be **proposed**, **accepted** or **deprecated**
- a **context** section which describes **constraints** guiding the decision: these could be technological, political or project local.
- a **decision** section which describes the solution resulting from the constraints described before.
- a **consequence** section which describes the resulting context e.g. advantages and drawbacks / limitations of the solution. Ideas for future improvement could be described shortly here.

# Summary

- [Kernel initialization](./adr-001.md)
- [Task creation and context switching](./adr-002.md)
- [Init task](./adr-003.md)
- [Build system](./adr-004.md)
- [Anckor test engine](./adr-005.md)
- [Continous integration](./adr-006.md)
- [Exceptions](./adr-007.md)
- [syscalls](./adr-008.md)
- [Prevent tasks to run in kernel mode](./adr-009.md)
- [Interrupts](./adr-010.md)
- [Synchronous message passing](./adr-011.md)
- [Dynamic memory allocation](./adr-012.md)