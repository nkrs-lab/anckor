# Anckor

![Build Status](https://github.com/nkrs-lab/anckor/actions/workflows/build_and_test.yaml/badge.svg)

Anckor aims to be a real-time kernel with a focus on **determinism**, **robustness** and **simplicity**.

The main goal is to design a lightweight kernel with robust partitioning between processes. Its architecture and API has to be as simple as possible to lower the cost of application design and maintenance. A particular attention is done on keeping as fewer system calls as possible.

All design choices are recorded following the [ADR](./doc/arch/README.md) framework.

## Installation

First install the **cross-toolchain** and **Qemu** for riscv:

```shell
$ sudo apt-get update
$ sudo apt install qemu-system-riscv64
$ sudo apt install gcc-riscv64-unknown-elf
```

And download the project sources:

```shell
$ git clone https://github.com/qoda-dev/anckor.git
```

Then run the **setenv.sh** script: 

```shell
$ source ./setenv.sh
...
Successfully installed anckor-CLI-0.0.1
```

Now you can use the **anckor** meta tool to **configure**, **build** and **run** the project. Default target is **qemu-riscv64**.

```shell
$ anckor configure
[CONFIGURE]
use config: default_defconfig
generate config.mk file

$ anckor build
[BUILD]
...
link all objects files
generate kernel image

$ anckor run
[RUN]
run [release] build
Anckor OS build r-0.0.2 - 2024-11-27 08:18:31
ATE - Anckor test engine
...
ATE - PASSED - 4 passed - 0 failed
```

With the default configuration, the kernel builds and runs the [tests](./tests/).

## Getting started

The kernel has an example you can use as a basis for your own application : 

```shell
$ anckor configure --config helloworld_defconfig

$ anckor build

$ anckor run
...
Anckor OS build r-0.0.2 - 2024-12-08 14:53:21
Hello world !
```

This configuration automatically disables tests and builds the code saved in [examples](./examples/). Modify it to suit your needs.

All kernel APIs are described [here](./doc/api/SUMMARY.md).

## Features

* **Hard Real-Time**. The kernel uses a tickless scheduler with fixed priorities. This design choice guarantees a high degree of predictability in system behaviour. All context switches are executed in **cooperative** mode. The only exception to this rule are **[interrupts](./doc/arch/adr-010.md)** which are designed to **[wakeup](./doc/api/task.md)** a driver task and call the scheduler. So an interrupt can preempt a task if it's corresponding driver task has a higher priority.
* **Fully Re-entrant Kernel**. As described in the **[interrupt architecture note](./doc/arch/adr-010.md)**, the kernel can be interrupted at any moment even if it runs in kernel mode.
* **Synchronous Message Passing**. The kernel uses a lightweight **[communication mechanism](./doc/arch/adr-011.md/)** to exchange data between tasks. This API is used as well for synchronisation as data passing between tasks.

## [Roadmap](/ROADMAP.md)