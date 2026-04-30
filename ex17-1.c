#include <acl/libacl.h>
#include <sys/acl.h>
#include <stdlib.h>
#include "ugid_functions.h"
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag;
    uid_t *uidp;
    gid_t *gidp;
    acl_permset_t permset;
    char *name, *pathname;
    int entryId, permVal;
    uid_t u = -1;
    gid_t g = -1;
    int id;
    int match;

    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname {u|g} id\n", argv[0]);

    pathname = argv[1];

    id = atoi(argv[3]);
    switch(argv[2][0]) {
    case 'u':
        if(id == 0)
            u = userIdFromName(argv[3]);
        else
            u = (uid_t)id;

        if (u == (uid_t)-1 || userNameFromId(u) == NULL)
            usageErr("%s is not a valid user id/name\n", argv[3]);

        break;
    case 'g':
        if(id == 0)
            g = groupIdFromName(argv[3]);
        else
            g = (gid_t)id;

        if (g == (gid_t)-1 || groupNameFromId(g) == NULL)
            usageErr("%s is not a valid group id/name\n", argv[3]);
        break;

    default:
        usageErr("%s pathname {u|g} {idname}\n", argv[0]);
        break;
    }


    type = ACL_TYPE_ACCESS;

    acl = acl_get_file(pathname, type);
    if (acl == NULL)
        errExit("acl_get_file");

    /* Walk through each entry in this ACL */

    for (entryId = ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY) {

        match = 0;
        if (acl_get_entry(acl, entryId, &entry) != 1)
            break;                      /* Exit on error or no more entries */

        /* Retrieve and display tag type */

        if (acl_get_tag_type(entry, &tag) == -1)
            errExit("acl_get_tag_type");

        if (u != (uid_t)-1 && tag == ACL_USER) {

            uidp = acl_get_qualifier(entry);
            if (uidp == NULL)
                errExit("acl_get_qualifier");

            if( u == *uidp) {
                match = 1;
                printf("%-12s", "user");
                name = userNameFromId(*uidp);
                if (name == NULL)
                    printf("%-8d ", *uidp);
                else
                    printf("%-8s ", name);
            }
            if (acl_free(uidp) == -1)
                errExit("acl_free");
        } else if (g != (gid_t)-1 && tag == ACL_GROUP) {

            gidp = acl_get_qualifier(entry);
            if (gidp == NULL)
                errExit("acl_get_qualifier");

            if( g == *gidp) {
                match = 1;
                printf("%-12s", "group");
                name = groupNameFromId(*gidp);
                if (name == NULL)
                    printf("%-8d ", *gidp);
                else
                    printf("%-8s ", name);

            }
            if (acl_free(gidp) == -1)
                errExit("acl_free");
        } else
            continue;


        if(match) {
            if (acl_get_permset(entry, &permset) == -1)
                errExit("acl_get_permset");

            permVal = acl_get_perm(permset, ACL_READ);
            if (permVal == -1)
                errExit("acl_get_perm - ACL_READ");
            printf("%c", (permVal == 1) ? 'r' : '-');
            permVal = acl_get_perm(permset, ACL_WRITE);
            if (permVal == -1)
                errExit("acl_get_perm - ACL_WRITE");
            printf("%c", (permVal == 1) ? 'w' : '-');
            permVal = acl_get_perm(permset, ACL_EXECUTE);
            if (permVal == -1)
                errExit("acl_get_perm - ACL_EXECUTE");
            printf("%c", (permVal == 1) ? 'x' : '-');
        }

        printf("\n");
    }

    if (acl_free(acl) == -1)
        errExit("acl_free");

    exit(EXIT_SUCCESS);
}
