#include "platform.h"
#include "visibility.h"
#include <stdlib.h>

Visible char *OPTbwsdir= (char *) NULL;
                        /* -g OPTbwsdir: group name workspaces */
Visible char *OPTworkspace= (char *) NULL;
                        /* -w OPTworkspace: start workspace */
Visible char *OPTcentral= (char *) NULL;
                        /* -c OPTcentral: central workspace */

Visible char *OPTeditor= (char *) NULL;
                        /* -e: use ${EDITOR} instead of ABC-editor */

Visible bool OPTunpack= No;
                       /* -u: unpack workspace */

Visible bool OPTslowterminal= No;
                        /* -s: do not tell "cannot insert" on slow terminal */

Visible int abc_todo;
                       /* task todo according to the arguments */
                       /* possible values are in port.h */
