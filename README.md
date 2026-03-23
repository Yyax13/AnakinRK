# AnakinRK – A Linux Kernel Rootkit using ftrace

AnakinRK is a lightweight Linux kernel module that demonstrates rootkit techniques by hooking system calls via **ftrace**. It provides privilege escalation, self‑hiding capabilities, and the ability to block the loading of other kernel modules. The project is intended strictly for educational and research purposes.

## Features

* **Root Privilege Escalation**
  Privileges are granted using a signal‑based command system hooked through `sys_kill`.

* **Module Visibility Toggle**
  The module can hide or unhide itself from `/proc/modules` and `lsmod`.

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
├── Makefile
├── build/                      # Compiled anakin.ko
├── .build-artifacts/           # Intermediate object files
└── src/
    ├── Makefile
    ├── ftrace_helper.c
    ├── ftrace_helper.h
    └── rk.c                    # Main rootkit logic
```

---

## Building

Make sure kernel headers for the running kernel are installed.

```bash
make
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

The rootkit hooks `sys_kill` and listens for a **magic signal (67)**.
Commands are triggered by sending this signal with a specific **PID value**.

### Magic Signal

```
67
```

---

## Commands

### 1. Get Root

```bash
kill -67 $(printf "%d" 0x$(printf "root" | xxd -p))
```

Grants root privileges (UID/GID = 0) to the calling process.

---

### 2. Hide / Unhide Module

```bash
kill -67 $(printf "%d" 0x$(printf "hide" | xxd -p))
```

Toggles visibility of the module from:

* `lsmod`
* `/proc/modules`

---

### 3. Show Help Menu (Kernel Log)

```bash
kill -67 $(printf "%d" 0x$(printf "help" | xxd -p))
```

Prints a command table to `dmesg`.

---

## Example

```bash
kill -67 $(printf "%d" 0x$(printf "help" | xxd -p))
dmesg | tail
```

---

## How It Works

The module installs three hooks using ftrace:

* `__x64_sys_kill`
* `__x64_sys_init_module`
* `__x64_sys_finit_module`

When signal `67` is detected, the PID value is interpreted as a command:

| PID value (hex) | Command     |
| --------------- | ----------- |
| `0x726f6f74`    | Get root    |
| `0x68696465`    | Toggle hide |
| `0x68656c70`    | Help menu   |

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