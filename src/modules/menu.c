#include "../include_modules/menu.h"
#include "../include/core.h"

int menu(long sig, long pid) {
#define X(cmd, magic, func, desc, err)                                         \
    if (sig == magic) {                                                        \
        if (cmd > 0) {                                                         \
            if ((int)pid == cmd) {                                             \
                func;                                                          \
                return err;                                                    \
            }                                                                  \
        } else {                                                               \
            func;                                                              \
            return err;                                                        \
        }                                                                      \
    }
    PIDTABLE
#undef X

    return 0;
}

void helpMenu(void) {
    printk(KERN_DEBUG "\n\n=========== AnakinRK Help Menu ===========\n");
    printk(KERN_DEBUG "=\tCMD\tSignal\tDescription\t=\n");
    printk(KERN_DEBUG "==========================================\n");

    char _cmd[5];
#define X(cmd, magic, func, desc, err)                                         \
    _cmd[0] = (cmd >> 24) & 0xFF;                                              \
    _cmd[1] = (cmd >> 16) & 0xFF;                                              \
    _cmd[2] = (cmd >> 8) & 0xFF;                                               \
    _cmd[3] = cmd & 0xFF;                                                      \
    _cmd[4] = '\0';                                                            \
    if (cmd < 0) {                                                             \
        _cmd[0] = 'n';                                                         \
        _cmd[1] = 'o';                                                         \
        _cmd[2] = 'n';                                                         \
        _cmd[3] = 'e';                                                         \
        _cmd[4] = '\0';                                                        \
    }                                                                          \
    printk(KERN_DEBUG "=\t%s\t%d\t%s\t=\n", _cmd, magic, desc);
    PIDTABLE
#undef X

    printk(KERN_DEBUG "==========================================\n\n\n");
};
