DEBUG_FLAG ?= false

include tools/generated/debug.mk

ifeq ($(DEBUG_FLAG), true)
	DEBUG_OPTIONS := -s -S
	BUILD_TYPE := --debug
else 
	DEBUG_OPTIONS :=
	BUILD_TYPE := --release
endif

run:
	$(info [run] $(BUILD_TYPE) build)
	@qemu-system-riscv64 $(DEBUG_OPTIONS) -machine virt -cpu rv64 -smp 4 -m 512M -nographic -serial mon:stdio -bios none -kernel build/anckor.img