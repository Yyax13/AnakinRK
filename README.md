# AnakinRK – A Linux Kernel Rootkit using ftrace

AnakinRK is a lightweight Linux kernel module that demonstrates rootkit techniques by hooking system calls via **ftrace**. It can grant root privileges, hide its own presence, and block the loading of other kernel modules. The code is intended for educational and research purposes only.

## Features

- **Root Privilege Escalation** – Send signal `66` to the magic PID (`1337`) to gain root (UID=0) in the calling process.
- **Module Visibility Toggle** – Send signal `65` to the magic PID to hide or unhide the module from `/proc/modules` and `lsmod`.
- **Module Loading Prevention** – Hooks `init_module` and `finit_module` so that any attempt to load a new kernel module silently succeeds but does nothing (i.e., modules are not actually loaded).
- **Cross‑version Compatibility** – Uses `kallsyms_lookup_name` or, on kernels ≥5.7, a kprobe to locate symbols.
- **ftrace‑based Hooking** – Modifies function pointers at runtime without needing `/dev/mem` or direct syscall table manipulation.

## Files

```
.
├── Makefile                    # Top-level build script
├── build/                      # Output directory for anakin.ko
├── .build-artifacts/           # Intermediate object files (ignored)
└── src/
    ├── Makefile                # Kernel module Makefile
    ├── ftrace_helper.c         # ftrace hooking helper functions
    ├── ftrace_helper.h         # Header for ftrace_helper
    └── rk.c                     # Main rootkit logic
```

## Building

Ensure you have the kernel headers installed for your running kernel.

```bash
make
```

The compiled module will be placed in `build/anakin.ko`.

## Loading / Unloading

- **Load** the module:
  ```bash
  sudo insmod build/anakin.ko
  ```
- **Unload** it (if not hidden):
  ```bash
  sudo rmmod anakin
  ```

After loading, the rootkit is active. It will print a message to the kernel log (visible with `dmesg`):

```
Anakin is here
```

Upon unload:

```
Anakin killed padmé
```

## Usage

The rootkit responds to signals sent to the **magic PID 1337**. You can use any process with that PID, or simply create a dummy process that sleeps.

### 1. Get Root

Send signal `66` to PID `1337`. This will give root privileges to the process that sends the signal.

Example (using `kill`):
```bash
kill -66 1337
```
After this command, the shell (or the process you used) will have UID 0. You can verify with `id`.

### 2. Toggle Module Visibility

Send signal `65` to PID `1337` to hide or unhide the module from the kernel module list. When hidden, the module will not appear in `lsmod` or `/proc/modules`.

```bash
kill -65 1337
```

The module’s visibility toggles each time the signal is received.

### Blocking Module Loading

Once the rootkit is loaded, any attempt to insert a new kernel module (via `insmod` or `modprobe`) will appear to succeed but the module will not actually be loaded. This is because the hooked `init_module` and `finit_module` syscalls simply return `0` without calling the original functions.

## Important Notes

- **Kernel Versions**  
  The code has been tested on various kernels up to 6.12. The macro `USE_FENTRY_OFFSET` in `ftrace_helper.h` is set to `0` to avoid crashes on newer kernels. If you encounter issues, you may need to adjust this value.

- **Magic PID**  
  The hard‑coded PID `1337` can be changed in `src/rk.c` by modifying `MAGIC_PID`. Recompile after any changes.

- **Visibility**  
  Hiding only removes the module from the kernel’s module list. It does **not** hide the kernel module file from the filesystem, nor does it prevent detection by more advanced rootkit scanners (e.g., those that check ftrace handlers, kprobes, or system call hooks).

- **Safety**  
  This code is for educational purposes. Using it on production systems or without authorisation is illegal and unethical.

## Disclaimer

This project is intended **only** for educational and research purposes. The authors are not responsible for any misuse or damage caused by this software. Always obey local laws and regulations.

---

![You understimate my power!](./assets/its-over-anakin.gif)
