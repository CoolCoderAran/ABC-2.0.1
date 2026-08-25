#ifndef ABC_UNIX_PLATFORM_HEADER
#define ABC_UNIX_PLATFORM_HEADER

/** -g OPTbwsdir: group name workspaces */
extern char *OPTbwsdir;
/** -w OPTworkspace: start workspace */
extern char *OPTworkspace;
/** -c OPTcentral: central workspace */
extern char *OPTcentral;

/** -e: use ${EDITOR} instead of ABC-editor */
extern char *OPTeditor;

/** -u: unpack workspace */
extern bool OPTunpack;

/** -s: do not tell "cannot insert" on slow terminal */
extern bool OPTslowterminal;

/**
 * task todo according to the arguments
 * possible values are in port.h
 */
extern int abc_todo;

#endif /* HEADER GUARD */
